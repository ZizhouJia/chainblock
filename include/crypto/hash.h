#pragma once
#include<openssl/sha.h>
#include<string>

namespace crypto{
  std::string sha256(const unsigned char* content,int size);
  std::string sha256(const std::string& content);
  std::string hash0();
}
