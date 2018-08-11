#include "block/block_lite.h"
#include "crypto/hash.h"
#include "crypto/utils.h"
#include<iostream>
#include<string>
#include<string.h>
namespace block{
  bool check_zero(const char* hash,int hardness){
    unsigned char init=0x00;
    int i=0;
    while(i<hardness){
      init=init|*((unsigned char*)(hash+i));
      ++i;
    }
    return init==0;
  }

  bool block_lite::calculate_nonce(){
    std::string content=version+chain_name+std::to_string(block_number)+prev_hash+time+hash_content_root+hash_exp_root;
    std::string hash_current=crypto::sha256(content);
    unsigned char to_hash[36]={0};
    memcpy(to_hash,hash_current.data(),32);
    unsigned int* number=(unsigned int*)(to_hash+32);
    while(*number<0xffffffff){
      std::string hash1=crypto::sha256(to_hash,36);
      std::string hash2=crypto::sha256(hash1);
      if(check_zero(hash2.c_str(),hardness)){
        this->nonce=(*number);
        this->block_hash=hash2;
        return true;
      }
      ++(*number);
    }
    return false;
  }

  std::string block_lite::get_hash(){
    if(block_hash==""){
      std::cout<<"hash is not set please try the calculate_nonce_number"<<std::endl;
      return "";
    }
    return block_hash;
  }

  bool block_lite::verify_the_block_hash(){
    std::string content=version+chain_name+std::to_string(block_number)+prev_hash+time+hash_content_root+hash_exp_root;
    std::string hash_current=crypto::sha256(content);
    unsigned char to_hash[36]={0};
    memcpy(to_hash,hash_current.data(),32);
    unsigned int* number=(unsigned int*)(to_hash+32);
    *number=this->nonce;
    std::string hash1=crypto::sha256(to_hash,36);
    std::string hash2=crypto::sha256(hash1);
    if(check_zero(hash2.c_str(),hardness)){
      return true;
    }
    return false;
  }

}
