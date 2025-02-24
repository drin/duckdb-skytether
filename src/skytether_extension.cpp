// ------------------------------
// License
//
// Copyright 2024 Aldrin Montana
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


// ------------------------------
// Dependencies

#define DUCKDB_EXTENSION_MAIN

#include "skytether_extension.hpp"


#ifndef DUCKDB_AMALGAMATION
  #include "duckdb/common/shared_ptr.hpp"
  #include "duckdb/common/exception.hpp"
  #include "duckdb/common/string_util.hpp"
  #include "duckdb/common/enums/optimizer_type.hpp"
  
  #include "duckdb/main/config.hpp"
  #include "duckdb/main/connection.hpp"
  #include "duckdb/main/client_context.hpp"
  #include "duckdb/main/extension_util.hpp"
  
  #include "duckdb/function/table_function.hpp"
  #include "duckdb/function/scalar_function.hpp"
  
  #include "duckdb/storage/storage_extension.hpp"

  #include "duckdb/parser/parsed_data/attach_info.hpp"
  #include "duckdb/parser/parsed_data/create_table_function_info.hpp"
  #include "duckdb/parser/parsed_data/create_pragma_function_info.hpp"
  #include "duckdb/parser/parsed_data/create_scalar_function_info.hpp"
#endif


// >> Wrap our custom catalog and transaction manager in a StorageExtension
namespace duckdb {

  //! Attach to skytether by constructing a `SkytetherCatalog`.
  unique_ptr<Catalog> AttachSkytether( StorageExtensionInfo* storage_info
                                      ,ClientContext&        context
                                      ,AttachedDatabase&     db
                                      ,const string&         name
                                      ,AttachInfo&           info
                                      ,AccessMode            access_mode) {
    return make_uniq<SkytetherCatalog>(db, info.path, access_mode);
  }
  
  //! Construct a transaction manager and initialize it with a `SkytetherCatalog`.
  unique_ptr<TransactionManager> CreateSkytetherTxMgr( StorageExtensionInfo* storage_info
                                                      ,AttachedDatabase&     db
                                                      ,Catalog&              catalog) {
    auto& sky_catalog = catalog.Cast<SkytetherCatalog>();
    return make_uniq<SkytetherTransactionManager>(db, sky_catalog);
  }

  //! This storage extension provides an interface to data stored in skytether by
  //  attaching to a custom catalog that implements reading from domains, partitions, and
  //  slices as if they were schemas and tables.
  //
  //  The `attach` attribute is a function pointer, duckdb::attach_function_t, defined in
  //  the base storage extension header. The `create_transaction_manager` is also a
  //  function pointer, duckdb::create_transaction_manager_t, defined in the same header.
  //  base storage extension header: "duckdb/storage/storage_extension.hpp"
  struct SkytetherStorageExtension : public StorageExtension {
    SkytetherStorageExtension() {
      attach                     = AttachSkytether;
      create_transaction_manager = CreateSkytetherTxMgr;
    }
  };

} // namespace: duckdb
  

// ------------------------------
// Implementation for extension framework

namespace duckdb {

  //! Register our storage extension using `SKYTETHER_EXTNAME`.
  //
  //  This is used by duckdb to find our extension when creating an attached database
  //  with: AttachOptions.db_type = <ext_name>
  static void LoadInternal(DatabaseInstance& instance) {
    auto& config = DBConfig::GetConfig(instance);
    config.storage_extensions[SKYTETHER_EXTNAME] = make_uniq<SkytetherStorageExtension>();
  }
  
  void        SkytetherExtension::Load(DuckDB &db) { LoadInternal(*db.instance); }
  std::string SkytetherExtension::Name()           { return SKYTETHER_EXTNAME;   }
  std::string SkytetherExtension::Version() const  { return SKYTETHER_EXTVERS;   }

} // namespace duckdb


// >> c-api functions for initializing extension and getting version
extern "C" {

  DUCKDB_EXTENSION_API
  void skytether_init(duckdb::DatabaseInstance& db) {
    duckdb::DuckDB db_wrapper(db);
    db_wrapper.LoadExtension<duckdb::SkytetherExtension>();
  }
  
  DUCKDB_EXTENSION_API
  const char *skytether_version() {
    return duckdb::DuckDB::LibraryVersion();
  }

}

#ifndef DUCKDB_EXTENSION_MAIN
  #error DUCKDB_EXTENSION_MAIN not defined
#endif

