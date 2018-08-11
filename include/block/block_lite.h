#pragma once
#include<string>
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
    int hardness=3;
  public:
    block_lite(){};
    block_lite(std::string chain_name,int block_number,std::string prev_hash,std::string time):
    chain_name(chain_name),block_number(block_number),prev_hash(prev_hash),time(time){};
    std::string get_hash();
    bool calculate_nonce();
    bool verify_the_block_hash();
  };
}
