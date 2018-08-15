#include<string>
#include<vector>
#include<map>
#include<iostream>
#include<algorithm>

#include "db/database_manager.h"
#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"

namespace db{
  std::string get_key(const int block_number,const int item_index){
    return std::to_string(block_number)+":"+std::to_string(item_index);
  }

  int db_manager::global_init(const std::string& path){
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status s = rocksdb::DB::Open(options, path, &db);
    if(s.ok()){
      return 1;
    }
    return 0;
  }

  db_manager::~db_manager(){
    for(auto it=rocks_handles.begin();it!=rocks_handles.end();++it){
      delete it->second;
    }
    delete db;
  }

  int db_manager::create_new_chain_db(const std::string& chain_name){
    if(std::find(chain_names.begin(),chain_names.end(),chain_name)!=chain_names.end()){
      std::cout<<"the chain has already exist"<<std::endl;
      return 0;
    }
    std::string block_head=chain_name+":"+"block_head";
    std::string block_trx=chain_name+":"+"block_trx";
    std::string block_trx_prev=chain_name+":"+"block_trx_prev";
    std::string block_trx_next=chain_name+":"+"block_trx_next";
    std::vector<std::string> names;
    names.push_back(block_head);
    names.push_back(block_trx);
    names.push_back(block_trx_prev);
    names.push_back(block_trx_next);
    std::vector<rocksdb::ColumnFamilyHandle*> handles;
    rocksdb::Status s;
    for(int i=0;i<names.size();++i){
      rocksdb::ColumnFamilyHandle* cf;
      s=db->CreateColumnFamily(rocksdb::ColumnFamilyOptions(),names[i], &cf);
      if(s.ok()){
        handles.push_back(cf);
      }else{
        std::cout<<s.getState()<<std::endl;
        for(int j=0;j<handles.size();++j){
          s = db->DropColumnFamily(handles[j]);
          if(!s.ok()){
            std::cout<<"error in create_new_chain_db"<<std::endl;
            return 0;
          }
        }
        std::cout<<"the create_new_chain_db unsucceed"<<std::endl;
        return 0;
      }
    }
    for(int i=0;i<names.size();++i){
      rocks_handles[names[i]]=handles[i];
    }
    chain_names.push_back(chain_name);
    return 1;
  }

  int db_manager::add_header(const std::string& chain_name,const int block_number,const std::string& header_json){
    std::string key=std::to_string(block_number);
    std::string handle_name=chain_name+":"+"block_head";
    rocksdb::Status s = db->Put(rocksdb::WriteOptions(),rocks_handles[handle_name], rocksdb::Slice(key), rocksdb::Slice(header_json));
    if(s.ok()){
      return 1;
    }
    return 0;
  }

  int db_manager::get_header(const std::string& chain_name,const int block_number,std::string& header_json){
    std::string key=std::to_string(block_number);
    std::string value;
    std::string handle_name=chain_name+":"+"block_head";
    rocksdb::Status s=db->Get(rocksdb::ReadOptions(),rocks_handles[handle_name],rocksdb::Slice(key),&value);
    if(s.ok()){
      header_json=value;
      return 1;
    }else{
      std::cout<<s.ToString()<<std::endl;
      return 0;
    }

  }

  int db_manager::add_item(const std::string& chain_name,const int block_number,
    const int item_index,const std::string& item_json){
        std::string handle_name=chain_name+":"+"block_item";
        rocksdb::Status s=db->Put(rocksdb::WriteOptions(),rocks_handles[handle_name],rocksdb::Slice(get_key(block_number,item_index)),
        rocksdb::Slice(item_json));
        if(s.ok()){
          return 1;
        }
        return 0;
  }

  int db_manager::get_item(const std::string& chain_name,const int block_number,const int item_index,std::string& item_json){
    std::string key=get_key(block_number,item_index);
    std::string value;
    std::string handle_name=chain_name+":"+"block_item";
    rocksdb::Status s=db->Get(rocksdb::ReadOptions(),rocks_handles[handle_name],rocksdb::Slice(key),&value);
    if(s.ok()){
      item_json=value;
      return 1;
    }else{
      std::cout<<s.ToString()<<std::endl;
      return 0;
    }

  }

  int db_manager::check_last(const std::string& chain_name,const std::string key){
    std::string value;
    std::string handle_name=chain_name+":"+"block_next";
    rocksdb::Status s=db->Get(rocksdb::ReadOptions(),rocks_handles[handle_name],rocksdb::Slice(key),&value);
    if(s.ok()){
      if(value=="-1"){
        return 1;
      }else{
        return 0;
      }
    }else{
      std::cout<<s.ToString()<<std::endl;
      return 0;
    }
  }
  int db_manager::set_prev(const std::string& chain_name,
    const int block_number,const int item_index,const std::string prev_key){
          std::string handle_name=chain_name+":"+"block_prev";
          rocksdb::Status s=db->Put(rocksdb::WriteOptions(),rocks_handles[handle_name],rocksdb::Slice(get_key(block_number,item_index)),
          rocksdb::Slice(prev_key));
          if(s.ok()){
            return 1;
          }
          return 0;
  }
  int db_manager::set_next(const std::string& chain_name,
    const std::string& current,const int block_number,const int item_index){
            std::string handle_name=chain_name+":"+"block_next";
            rocksdb::Status s=db->Put(rocksdb::WriteOptions(),rocks_handles[handle_name],rocksdb::Slice(current),
            rocksdb::Slice(get_key(block_number,item_index)));
            if(s.ok()){
              return 1;
            }
            return 0;
  }

  std::string db_manager::prev(const std::string& chain_name,const std::string& current_key){
    std::string value;
    std::string handle_name=chain_name+":"+"block_prev";
    rocksdb::Status s=db->Get(rocksdb::ReadOptions(),rocks_handles[handle_name],rocksdb::Slice(current_key),&value);
    if(s.ok()){
      return value;
    }else{
      std::cout<<s.ToString()<<std::endl;
      return 0;
    }

  }

}
