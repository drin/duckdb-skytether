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

// >> DuckDB deps
#include "apidep_duckdb.hpp"

// >> Internal deps
#include "skytether_api.hpp"


// ------------------------------
// Aliases and Type forwards

namespace duckdb {

  // >> Convenience aliases for our custom catalog
  using MaybeEntry       = optional_ptr<CatalogEntry>;
  using MaybeSchemaEntry = optional_ptr<SchemaCatalogEntry>;
  using CatalogCallback  = std::function<void(CatalogEntry &)>;

  // essentially a map<string, T>
  using DomainMap    = case_insensitive_map_t<unique_ptr<CatalogEntry>>;
  using PartitionMap = case_insensitive_map_t<unique_ptr<CatalogEntry>>;


  // Type forwards
  struct DropInfo;
  struct CreateSchemaInfo;
  struct CreateTableInfo;

  class SkytetherTransaction;
  class SkytetherSchemaEntry;
  class SkytetherResult;

} // namespace: duckdb
