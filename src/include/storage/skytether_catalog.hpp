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

#include "storage/skytether_catalog_set.hpp"


// ------------------------------
// Classes to support skytether catalog and storage interface

namespace duckdb {
  
  // >> Custom database catalog
  struct SkytetherCatalog : public Catalog {

    // >> attributes
    private SkytetherDomainSet active_domain;
    private string             default_schema;

    // >> destructors/constructors
    ~SkytetherCatalog() = default;
    explicit SkytetherCatalog(AttachedDatabase& db);
  
    // >> methods
    // administrative methods
    bool   IsDuckCatalog()  override { return false;       }
    string GetCatalogType() override { return "skytether"; }
  
    bool         InMemory()                              override;
    string       GetDBPath()                             override;
    void         Initialize(bool load_builtin)           override;
    DatabaseSize GetDatabaseSize(ClientContext &context) override;
  
    // schema management methods
    MaybeEntry CreateSchema(CatalogTransaction transaction, CreateSchemaInfo &info) override;
  
    void
    ScanSchemas(ClientContext& context, std::function<void(SchemaCatalogEntry&)> callback) override;
  
    MaybeSchemaEntry
    GetSchema( CatalogTransaction transaction
              ,const string&      schema_name
              ,OnEntryNotFound    if_not_found
              ,QueryErrorContext  error_context = QueryErrorContext()) override;

    // logical and physical operator methods
    unique_ptr<LogicalOperator>
    BindCreateIndex( Binder&                     binder
                    ,CreateStatement&            stmt
                    ,TableCatalogEntry&          table
                    ,unique_ptr<LogicalOperator> plan) override;
  
    // TODO: understand what these are for
    // stored procedure management methods? maybe views
    unique_ptr<PhysicalOperator>
    PlanCreateTableAs( ClientContext&               context
                      ,LogicalCreateTable&          op
                      ,unique_ptr<PhysicalOperator> plan) override;

    unique_ptr<PhysicalOperator>
    PlanInsert( ClientContext&               context
               ,LogicalInsert&               op
               ,unique_ptr<PhysicalOperator> plan) override;

    unique_ptr<PhysicalOperator>
    PlanUpdate( ClientContext&               context
               ,LogicalUpdate&               op
               ,unique_ptr<PhysicalOperator> plan) override;

    unique_ptr<PhysicalOperator>
    PlanDelete( ClientContext&               context
               ,LogicalDelete&               op
               ,unique_ptr<PhysicalOperator> plan) override;

    private:
      void DropSchema(ClientContext& context, DropInfo& info) override;
      void Verify() override;
  };

} // namespace duckdb

