#include <leveldb/db.h>
#include <cassert>

int main() {
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "./db/testdb", &db);
    assert(status.ok());

    return 0;
}
