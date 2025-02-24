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
#pragma once

// common deps across headers
#include "skytether_common.hpp"

#include "storage/skytether_table_set.hpp"


// ------------------------------
// Classes

namespace duckdb {

  // A Skytether domain is equivalent to a fully qualified schema catalog entry.
  // This means that while DuckDB uses <db>.<schema>, Skytether uses <domain>.
  struct SkytetherDomain : public SchemaCatalogEntry {

    // >> attributes
    unique_ptr<SkytetherAPISchema> schema_data;

    private SkytetherTableSet tables;
  
    // >> destructors/constructors
    SkytetherDomain(Catalog& catalog, CreateSchemaInfo& info);
  
    // methods
    private SkytetherCatalogSet& GetCatalogSet(CatalogType type);

    // entry creation methods

    // unimplemented methods
    MaybeEntry    CreateTable( CatalogTransaction    transaction
                              ,BoundCreateTableInfo& info) override;
    MaybeEntry      CreateType( CatalogTransaction transaction
                               ,CreateTypeInfo&    info) override;
    MaybeEntry     CreateView( CatalogTransaction transaction
                              ,CreateViewInfo&    info) override;
    MaybeEntry    CreateIndex( CatalogTransaction transaction
                              ,CreateIndexInfo&   info
                              ,TableCatalogEntry& table) override;

    MaybeEntry       CreateFunction( CatalogTransaction        transaction
                                    ,CreateFunctionInfo&       info) override;
    MaybeEntry  CreateTableFunction( CatalogTransaction        transaction
                                    ,CreateTableFunctionInfo&  info) override;

    // methods that will not be implemented
    MaybeEntry CreateCollation( CatalogTransaction   transaction
                               ,CreateCollationInfo& info) override;
    MaybeEntry CreateSequence( CatalogTransaction  transaction
                              ,CreateSequenceInfo& info) override;

    MaybeEntry   CreateCopyFunction( CatalogTransaction        transaction
                                    ,CreateCopyFunctionInfo&   info) override;
    MaybeEntry CreatePragmaFunction( CatalogTransaction        transaction
                                    ,CreatePragmaFunctionInfo& info) override;


    // >> catalog entry methods
    // catalog entry management
    void     Alter(CatalogTransaction transaction, AlterInfo& info) override;
    void DropEntry(ClientContext&     context    , DropInfo&  info) override;

    // catalog entry access
    void Scan(CatalogType type, const CatalogCallback& callback) override;
    void Scan( ClientContext&         context
              ,CatalogType            type
              ,const CatalogCallback& callback) override;

    MaybeEntry GetEntry( CatalogTransaction transaction
                        ,CatalogType        type
                        ,const string&      name) override;
  };

} // namespace duckdb

