#pragma once
#include<string>
#include<vector>
#include<map>

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"

namespace db{
  class db_manager{
  public:
    int global_init(const std::string& path);
    ~db_manager();
    int create_new_chain_db(const std::string& chain_name);
    int add_header(const std::string& chain_name,const int block_number,const std::string& header_json);
    int get_header(const std::string& chain_name,const int block_number,std::string& header_json);
    int add_item(const std::string& chain_name,const int block_number,const int item_index,const std::string& item_json);
    int get_item(const std::string& chain_name,const int block_number,const int item_index,std::string& item_json);
    int check_last(const std::string& chain_name,const std::string key);
    int set_prev(const std::string& chain_name,const int block_number,const int item_index,const std::string prev_key);
    int set_next(const std::string& chain_name,const std::string& current,const int block_number,const int item_index);
    std::string prev(const std::string& chain_name,const std::string& current_key);
  private:
    rocksdb::DB* db;
    std::map<std::string,rocksdb::ColumnFamilyHandle*> rocks_handles;
    std::vector<std::string> chain_names;
  };
}
