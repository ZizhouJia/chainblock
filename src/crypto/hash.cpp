#include<string>
#include<openssl/sha.h>
#include "crypto/hash.h"
#include "crypto/utils.h"

namespace crypto{
  std::string sha256(const unsigned char* content,int size){
    unsigned char hash[33];
    SHA256(content,size,hash);
    hash[32]='\0';
    std::string result(reinterpret_cast<char*>(hash),32);
    return result;
  }
  
  std::string sha256(const std::string& content){
    unsigned char* in=tools::cast_uchar(content.c_str());
    unsigned char hash[33];
    SHA256(in,content.length(),hash);
    hash[32]='\0';
    std::string result(reinterpret_cast<char*>(hash),32);
    return result;
  }

  std::string hash0(){
    unsigned char hash[33]={0};
    std::string result(reinterpret_cast<char*>(hash),32);
    return result;
  }
}
