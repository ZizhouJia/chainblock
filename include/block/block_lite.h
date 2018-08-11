#pragma once
#include<string>
#include "crypto/utils.h"
namespace block{
  class block_lite{
  protected:
    std::string version="0.0.1";
    std::string chain_name;
    int block_number;
    unsigned int nonce=0;
    std::string prev_hash;
    std::string time;
    std::string hash_content_root;
    std::string hash_exp_root;
    std::string block_hash="";
    unsigned char to_hash[36];
    unsigned int* current;
    unsigned int rand_begin;
    int hardness=3;
  public:
    block_lite(){};
    block_lite(std::string chain_name,int block_number,std::string prev_hash,std::string time):
    chain_name(chain_name),block_number(block_number),prev_hash(prev_hash),time(time){
      current=(unsigned int*)(to_hash+32);
    };
    std::string get_hash();
    bool calculate_nonce(); //old interface
    bool verify_the_block_hash();
    void init_calculate();
    bool calculate_nonce_once();
  };
}
