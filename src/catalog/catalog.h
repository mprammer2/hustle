// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#ifndef HUSTLE_CATALOG_H
#define HUSTLE_CATALOG_H

#include <cereal/access.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>

#include "absl/strings/str_cat.h"
#include "gtest/gtest.h"
#include "sqlite3/sqlite3.h"
#include "storage/table.h"
#include "table_schema.h"
#include "utils/map_utils.h"
#include "utils/sqlite_utils.h"

using namespace hustle::storage;

namespace hustle {
namespace catalog {

class TableInfo {
  public:
    TableSchema table_schema;
    DBTable::TablePtr table;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(CEREAL_NVP(table_schema));
    }
};

class Catalog {
 public:
  static Catalog CreateCatalog(std::string CatalogPath, std::string SqlitePath);
  static std::shared_ptr<Catalog> CreateCatalogObject(std::string CatalogPath,
                                                      std::string SqlitePath);

  bool addTable(TableSchema t);
  bool addTable(TableSchema t, DBTable::TablePtr table_ref);

  bool dropMemTable(std::string name);

  bool dropTable(std::string name);

  std::optional<TableSchema*> TableExists(std::string name);

  void print() const;

  DBTable::TablePtr getTable(size_t table_id);
  DBTable::TablePtr getTable(std::string table_name);

  std::vector<std::string> getTables(){ 
    std::vector<std::string> result;
    std::transform(tables_.begin(), tables_.end(),
    std::inserter(result, result.end()),
      [](auto table_info) { return table_info.first; });
    return result;
  }

  std::vector<TableSchema> getTableSchemas(){ 
    std::vector<TableSchema> tables;
    std::transform(tables_.begin(), tables_.end(),
    std::inserter(tables, tables.end()),
      [](auto table_info) { return table_info.second.table_schema; });
    return tables;
  }

  // Used by cereal for serialization/deserialization
  template <class Archive>
  void serialize(Archive& archive) {
    std::vector<TableSchema> tables;

    archive(CEREAL_NVP(CatalogPath_), CEREAL_NVP(SqlitePath_),
     CEREAL_NVP(tables_));
  }

  Catalog(){};

  Catalog(std::string CatalogPath, std::string SqlitePath)
      : CatalogPath_(CatalogPath), SqlitePath_(SqlitePath){};

 private:
  // Serializes and writes the catalog to a file in json format.
  void SaveToFile();

  // TODO(chronis) make private
  std::vector<DBTable::TablePtr> table_refs_;
  std::map<std::string, TableInfo> tables_;
  std::string CatalogPath_;
  std::string SqlitePath_;

  FRIEND_TEST(CatalogTest, Serialization);
};

Catalog CatalogFactory(std::string DBPath);

}  // namespace catalog
}  // namespace hustle

#endif  // HUSTLE_CATALOG_H
