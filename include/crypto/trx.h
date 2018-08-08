#pragma once
#include<string>
#include<vector>

namespace trx{
  struct sign_info{
    std::string public_key;
    std::string personal_content;
    std::string time;
    std::string random_number;
    std::string sign;
  };
  class transaction{
  public:
    std::string content;
    std::string prev_trx;
    std::vector<sign_info> sign_infos;
    transaction(std::string& content,std::vector<std::string>& public_keys):content(content){
      for(auto it=public_keys.begin();it!=public_keys.end();++it){
        sign_info info;
        info.public_key=*it;
        sign_infos.push_back(info);
      }

    };
    transaction(){};
  };

  int sign_trx(transaction& trx,const std::string& personal_content,const std::string& public_key,const std::string& private_key);
  int verify_trx(const transaction& trx);
}
