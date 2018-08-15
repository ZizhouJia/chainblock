#include<rocksdb/slice.h>
#include<rocksdb/options.h>
#include<rocksdb/db.h>
#include<string>
#include<iostream>
#include<vector>

using namespace rocksdb;
int main(){
  std::string kDBPath = "./db";
  Options options;
  options.create_if_missing=true;
  DB* db;
  Status s=rocksdb::DB::Open(options,kDBPath,&db);
  ColumnFamilyHandle* cf;
  s=db->CreateColumnFamily(ColumnFamilyOptions(), "new_cf", &cf);
  delete cf;
  delete db;

  std::vector<ColumnFamilyDescriptor> column_families;
  column_families.push_back(ColumnFamilyDescriptor(kDefaultColumnFamilyName,ColumnFamilyOptions()));
  column_families.push_back(ColumnFamilyDescriptor("new_cf",ColumnFamilyOptions()));

  std::vector<ColumnFamilyHandle*> handles;
  s=DB::Open(DBOptions(),kDBPath,column_families,&handles,&db);
  s=db->Put(WriteOptions(),handles[1],Slice("key1"),Slice("Value"));
  std::string value;
  s=db->Get(ReadOptions(),handles[1],Slice("key1"),&value);
  std::cout<<value<<std::endl;
  delete db;



}
