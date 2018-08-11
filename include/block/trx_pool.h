#pragma once
#include<vector>
#include<string>
#include "crypto/trx.h"
#include "block/merkle_tree.h"
namespace block{
  class trx_pool{
  private:
    std::vector<trx::transaction> items;
    std::vector<std::string> exps;
  public:
    std::string get_hash_content_root();
    std::string get_hash_exp_root();
    merkle::merkle_tree* make_merkle_tree();
    void clean_the_pool();
    void add_member(trx::transaction items,std::string exps);
    std::vector<trx::transaction> get_trxs(){
      return items;
    }
    std::vector<std::string> get_exps(){
      return exps;
    }
  };
}
