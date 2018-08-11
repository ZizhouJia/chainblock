#pragma once
#include<string>
#include "crypto/trx.h"

namespace tools{
  unsigned char* cast_uchar(const char* str_c);
  std::string rand_number();
  unsigned int complex_rand_number();
  std::string get_time();
}

namespace crypto{
  int char2int(const char c);
  std::string bin2str(const std::string& hash);
  std::string str2bin(const std::string& str );

}

namespace trx{
  std::string trx2json(const trx::transaction& trans);
  transaction json2trx(const std::string& json);
}
