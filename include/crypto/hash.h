#pragma once
#include<openssl/sha.h>
#include<string>

namespace crypto{
  std::string sha256(const std::string& content);
  
}
