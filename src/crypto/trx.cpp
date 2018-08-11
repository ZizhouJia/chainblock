#include<string>
#include<vector>
#include<algorithm>
#include<iostream>
#include<crypto/utils.h>
#include<crypto/hash.h>
#include<crypto/trx.h>
#include<crypto/ecc.h>

namespace trx{

  std::string contact_public_keys(transaction trans){
    std::string pubkey_contact="";
    for(auto it=trans.sign_infos.begin();it!=trans.sign_infos.end();++it){
      pubkey_contact+=crypto::bin2str(it->public_key);
    }
    return pubkey_contact;
  }

  int sign_trx(transaction& trx,const std::string& personal_content,const std::string& public_key,const std::string& private_key){
    if(trx.sign_infos.size()==0){
      std::cout<<"no keys to sign"<<std::endl;
      return 0;
    }
    for(auto it=trx.sign_infos.begin();it!=trx.sign_infos.end();++it){
      if(it->public_key==public_key){
        std::string random_number=std::to_string(tools::complex_rand_number());
        std::string date=tools::get_time();
        std::string sign_content=trx.prev_trx+trx.content+contact_public_keys(trx)+personal_content+random_number+date;
        std::string hash=crypto::sha256(sign_content);
        // crypto::sha256(sign_content);
        std::string sign=crypto::sign(private_key,hash);
        if(sign==""){
          std::cout<<"fail to sign the content"<<std::endl;
        }
        it->time=date;
        it->random_number=random_number;
        it->personal_content=personal_content;
        it->sign=sign;
        return 1;
      }
    }
    std::cout<<"the public_key is not in the list"<<std::endl;
    return 0;
  }

  int verify_trx(const transaction& trx){
    if(trx.sign_infos.size()==0){
      std::cout<<"no sign to verify"<<std::endl;
      return 0;
    }
    std::string sign_content_prefix=(trx.prev_trx+trx.content+contact_public_keys(trx));
    for(int i=0;i<trx.sign_infos.size();i++){
      std::string sign_content=(sign_content_prefix+trx.sign_infos[i].personal_content+trx.sign_infos[i].random_number+trx.sign_infos[i].time);
      std::string hash=crypto::sha256(sign_content);
      if(!crypto::verify(trx.sign_infos[i].public_key,hash,trx.sign_infos[i].sign)){
        std::cout<<"fail to verify in index:"<<i<<std::endl;
        return 0;
      }
    }
    return 1;
  }
}
