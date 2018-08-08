#pragma once

#include<string>

namespace crypto{
  std::string generate_private_key();
  std::string generate_public_key(const std::string& private_key);
  std::string generate_address(std::string& public_key);
  std::string sign(const std::string& private_key,const std::string& hash);
  bool verify(const std::string& public_key,const std::string& hash,const std::string& sign);
}
