#include<string>
#include<iostream>
#include "db/database_manager.h"

void test_db_create(){
  db::db_manager manager;
  if(!manager.global_init("/home/jzz/chainblock/tests/db/data")){
    std::cout<<"fail on init db"<<std::endl;
    return;
  }
  int ret=manager.create_new_chain_db("test1");
  if(ret){
    std::cout<<"create test1 succeed"<<std::endl;
  }
  ret=manager.create_new_chain_db("test2");
  if(ret){
    std::cout<<"create test2 succeed"<<std::endl;
  }
  std::cout<<"add header"<<std::endl;
  if(!manager.add_header("test1",100,"write and get test1 succeed")){
    std::cout<<"fail on add header"<<std::endl;
  }
  if(!manager.add_header("test2",101,"write and get test2 succeed")){
    std::cout<<"fail on add header"<<std::endl;
  }
  std::cout<<"get header"<<std::endl;
  std::string value;
  if(!manager.get_header("test1",100,value)){
    std::cout<<"fail on get header"<<std::endl;
  }else{
    std::cout<<"succeed"<<std::endl;
    std::cout<<value<<std::endl;
  }

  if(!manager.get_header("test2",101,value)){
    std::cout<<"fail on get header"<<std::endl;
  }else{
    std::cout<<value<<std::endl;
  }
}


int main(){
  test_db_create();
}
