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

#include "storage/skytether_domain.hpp"
#include "storage/skytether_partition.hpp"


// ------------------------------
// Classes

namespace duckdb {
  
  struct SkytetherDomainSet {

    // >> Methods
    SkytetherDomainSet(Catalog& catalog);
  
    void Scan(ClientContext& context, const CatalogCallback& callback);

            void       ClearEntries();
            MaybeEntry GetEntry(ClientContext &context, const string &name);
    virtual MaybeEntry CreateEntry(unique_ptr<CatalogEntry> entry);
    virtual void       DropEntry(ClientContext& context, DropInfo& info);
    
    // >> Attributes
    protected Catalog&  catalog;
    private   DomainMap domains;
    private   bool      is_loaded;
  };
  
  struct SkytetherPartitionSet {

    // >> Methods
    SkytetherPartitionSet(Catalog& catalog);
  
    void Scan(ClientContext& context, const CatalogCallback& callback);

            void       ClearEntries();
            MaybeEntry GetEntry(ClientContext &context, const string &name);
    virtual MaybeEntry CreateEntry(unique_ptr<CatalogEntry> entry);
    virtual void       DropEntry(ClientContext& context, DropInfo& info);
    
    // >> Attributes
    protected Catalog&     catalog;
    private   PartitionMap partitions;
    private   bool         is_loaded;
  };
  
} // namespace: duckdb

