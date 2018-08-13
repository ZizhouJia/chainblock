#include<string>
#include<vector>
#include<algorithm>
#include<iostream>
#include<crypto/utils.h>
#include<crypto/hash.h>
#include<crypto/trx.h>
#include<crypto/ecc.h>

namespace trx{
  void transaction::add_singer(const signer &s){
    signers.push_back(s);
  }
  void transaction::add_signer(const std::string& public_key,const std::string& personal_content, const std::vector<std::string>& link_trx){
    signer s;
    s.public_key=public_key;
    s.personal_content=personal_content;
    s.link_trx=link_trx;
    signers.push_back(s);
  }

  void transaction::add_parties(const std::string& partie){
    parties.push_back(partie);
  }

  std::vector<signer> transaction::get_signers(){
    return signers;
  }

  std::vector<std::string> transaction::get_parties(){
    return parties;
  }

  void transaction::set_prev_trx(const std::string& prev_trx){
    this->prev_trx=prev_trx;
  }

  std::string transaction::get_prev_trx(){
    return prev_trx;
  }

  void transaction::set_chain_name(std::string chain_name){
    this->chain_name=chain_name;
  }

  std::string transaction::get_chain_name(){
    return chain_name;
  }

  void transaction::set_contract(std::string contract){
    this->contract=contract;
  }

  std::string transaction::get_contract(){
    return contract;
  }

  void transaction::set_action(std::string action){
    this->action=action;
  }

  std::string transaction::get_action(){
    return action;
  }

  void transaction::set_content(std::string content){
    this->content=content;
  }

  std::string transaction::get_content(){
    return content;
  }

  int transaction::get_index_by_public_key(const std::string& public_key){
    int index=-1;
    for(int i=0;i<signers.size();++i){
      if(signers[i].public_key==public_key){
        index=i;
        return index;
      }
    }
    return index;
  }

  void transaction::calculate_trans_content_hash(){
    if(trans_content_hash!=""){
      return;
    }
    std::string contents=(prev_trx+chain_name+contract+action+content);
    for(auto it=parties.begin();it!=parties.end();++it){
      contents+=(*it);
    }
    for(auto it=signers.begin();it!=signers.end();++it){
      contents+=(it->public_key);
      for(auto it2=it->link_trx.begin();it2!=it->link_trx.end();++it2){
        contents+=(*it2);
      }
    }
    trans_content_hash=crypto::sha256(contents);
  }

  void transaction::set_sign_random_number(const int index){
    signers[index].random_number=std::to_string(tools::complex_rand_number());
    signers[index].sign_time=tools::get_time();
  }

  std::string transaction::get_signer_hash(const int index){
    std::string personal_content=trans_content_hash+signers[index].personal_content+signers[index].sign_time+signers[index].random_number;
    return crypto::sha256(personal_content);
  }

  std::string transaction::calculate_sign(const std::string& public_key,std::string& private_key){
    int index=get_index_by_public_key(public_key);
    calculate_trans_content_hash();
    std::string hash=get_signer_hash(index);
    std::string sign=crypto::sign(private_key,hash);
    return sign;
  };

  int transaction::sign_trx(const std::string& public_key,std::string& private_key){
    int index=get_index_by_public_key(public_key);
    if(index<0){
      std::cout<<"can not find the public_key"<<std::endl;
      return 0;
    }
    set_sign_random_number(index);
    std::string sign=calculate_sign(public_key,private_key);
    signers[index].sign=sign;
    return 0;
  }

  bool transaction::verify_trx(){
    if(signers.size()==0){
      return false;
    }
    calculate_trans_content_hash();
    for(int i=0;i<signers.size();++i){
      std::string hash=get_signer_hash(i);
      if(!crypto::verify(signers[i].public_key,hash,signers[i].sign)){
        std::cout<<"unsucceed in verify index:"<<i<<",PBK:"<<crypto::bin2str(signers[i].public_key)<<std::endl;
        return false;
      }
    }
    return true;
  }

  std::string transaction::to_hash(){
      calculate_trans_content_hash();
      std::string contents=trans_content_hash;
      for(auto it=signers.begin();it!=signers.end();++it){
        contents+=(it->personal_content+it->sign_time+it->random_number+it->sign);
      }
      return crypto::sha256(contents);
  }

  std::string transaction::to_json(){
    return "";
  }

  transaction::transaction(std::string& json){

  }

}
