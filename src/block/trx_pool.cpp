#include "block/trx_pool.h"
#include "block/merkle_tree.h"
#include "crypto/hash.h"
#include<vector>
#include<string>

namespace block{
  std::string trx_pool::get_hash_content_root(){
    std::vector<std::string> hashs;
    for(int i=0;i<items.size();i++){
      hashs.push_back(items[i].to_hash());
    }
    return merkle::merkle_tree(hashs).get_root();
  };

  std::string trx_pool::get_hash_exp_root(){
    std::vector<std::string> hashs;
    for(int i=0;i<exps.size();i++){
      hashs.push_back(crypto::sha256(exps[i]));
    }
    return merkle::merkle_tree(hashs).get_root();
  }

  merkle::merkle_tree* trx_pool::make_merkle_tree(){
    std::vector<std::string> hashs;
    for(int i=0;i<items.size();i++){
      hashs.push_back(items[i].to_hash());
    }
    merkle::merkle_tree* ptr=new merkle::merkle_tree(hashs);
    return ptr;
  }

  void trx_pool::clean_the_pool(){
    this->items.clear();
    this->exps.clear();
  }
  void trx_pool::add_member(trx::transaction items,std::string exps){
    this->items.push_back(items);
    this->exps.push_back(exps);
  }

}
