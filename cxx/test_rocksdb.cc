#include <rocksdb/db.h>

#include "common/common.h"

namespace snippets {

TEST(RocksDB, OpenTest_00) {
  rocksdb::Options options;
  options.create_if_missing = true;

  rocksdb::DB* db{nullptr};
  std::string db_path = "/tmp/snippets_rocksdb_test_XXXXXX";
  ASSERT_TRUE(mkdtemp(&db_path[0]) != nullptr);
  VLOG(10) << "db_path " << db_path;

  rocksdb::Status s = rocksdb::DB::Open(options, db_path, &db);
  LOG_IF(ERROR, !s.ok()) << "db open failed, code" << s.code();
  ASSERT_TRUE(s.ok());
  std::unique_ptr<rocksdb::DB> guard(db);
}

}  // namespace snippets