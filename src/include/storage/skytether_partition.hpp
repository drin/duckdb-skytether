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


// ------------------------------
// Classes

// >> Representation of catalog entry for skytether slice
namespace duckdb {

  struct SkytetherSliceInfo {
    unique_ptr<CreateTableInfo> create_info;

    SkytetherSliceInfo() {
      create_info = make_uniq<CreateTableInfo>();
    }

    SkytetherSliceInfo(const string& schema, const string& table) {
      create_info = make_uniq<CreateTableInfo>(string(), schema, table);
    }

    SkytetherSliceInfo(const SchemaCatalogEntry& schema, const string& table) {
      create_info = make_uniq<CreateTableInfo>((SchemaCatalogEntry&) schema, table);
    }

    const string& GetTableName() const { return create_info->table; }
  };

  struct SkytetherSlice : public TableCatalogEntry {
    unique_ptr<SkytetherAPITable> table_data;

    SkytetherPartition(Catalog& catalog, SchemaCatalogEntry& schema, CreateTableInfo&    info);
    SkytetherPartition(Catalog& catalog, SchemaCatalogEntry& schema, SkytetherSliceInfo& info);

    unique_ptr<BaseStatistics>
    GetStatistics(ClientContext& context, column_t column_id) override;

    TableFunction
    GetScanFunction(ClientContext& context, unique_ptr<FunctionData>& bind_data) override;

    TableStorageInfo GetStorageInfo(ClientContext &context) override;

    void BindUpdateConstraints( Binder&            binder
                               ,LogicalGet&        get
                               ,LogicalProjection& proj
                               ,LogicalUpdate&     update
                               ,ClientContext&     context) override;
  };

} // namespace: duckdb


// >> Representation of catalog entry for skytether partition
namespace duckdb {

  struct SkytetherPartitionInfo {
    unique_ptr<CreateTableInfo> create_info;

    SkytetherPartitionInfo() {
      create_info = make_uniq<CreateTableInfo>();
    }

    SkytetherPartitionInfo(const string& schema, const string& table) {
      create_info = make_uniq<CreateTableInfo>(string(), schema, table);
    }

    SkytetherPartitionInfo(const SchemaCatalogEntry& schema, const string& table) {
      create_info = make_uniq<CreateTableInfo>((SchemaCatalogEntry&) schema, table);
    }

    const string& GetTableName() const { return create_info->table; }
  };

  struct SkytetherPartition : public TableCatalogEntry {
    unique_ptr<SkytetherAPITable> table_data;

    SkytetherPartition(Catalog& catalog, SchemaCatalogEntry& schema, CreateTableInfo&    info);
    SkytetherPartition(Catalog& catalog, SchemaCatalogEntry& schema, SkytetherPartitionInfo& info);

    unique_ptr<BaseStatistics>
    GetStatistics(ClientContext& context, column_t column_id) override;

    TableFunction
    GetScanFunction(ClientContext& context, unique_ptr<FunctionData>& bind_data) override;

    TableStorageInfo GetStorageInfo(ClientContext &context) override;

    void BindUpdateConstraints( Binder&            binder
                               ,LogicalGet&        get
                               ,LogicalProjection& proj
                               ,LogicalUpdate&     update
                               ,ClientContext&     context) override;

    void LoadEntries(ClientContext& context) override;
  };

} // namespace: duckdb
