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
// Overview

// The skytether extension implements a catalog (and related functionality) that is
// managed in a "skyhook-like manner." Specifically, this means that the storage manager
// is designed for an object storage system with a flat namespace that is likely distributed
// over many devices and does not fate-share with the compute node.
//
// Initially, this extension will support a storage model that is similar to Kinetic
// [cite], but will eventually also provide functionality to attach to, and leverage, a
// catalog backed by Kinetic.


// ------------------------------
// Dependencies
#pragma once

// common deps across headers
#include "skytether_common.hpp"

#include "storage/skytether_catalog.hpp"
#include "storage/skytether_transaction_manager.hpp"


// ------------------------------
// Macros

// TODO: make this configurable at build time
#define SKYTETHER_EXTVERS "0.1.0"
#define SKYTETHER_EXTNAME "skytether_catalog"


// ------------------------------
// Extension definition

namespace duckdb {

  //! A duckdb extension that registers a storage extension.
  //
  //  The storage extension, SkytetherStorageExtension, provides a catalog interface to
  //  a skytether storage system.
  struct SkytetherExtension : public Extension {
    void        Load(DuckDB& db) override;
    std::string Name()           override;
  };

} // namespace duckdb

