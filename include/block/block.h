#pragma once
#include<vector>
#include<string>
#include<memory>
#include "crypto/trx.h"
#include "block/merkle_tree.h"
#include "block/block_lite.h"
#include "block/trx_pool.h"
namespace block{
  class block:public block_lite{
  private:
    std::vector<trx::transaction> items;
    std::vector<std::string> exps;
    std::shared_ptr<merkle::merkle_tree> mtree;
  public:
    block(std::string chain_name,int block_number,std::string prev_hash,std::string block_time):block_lite(chain_name,block_number,prev_hash,block_time){};
    void set_block_from_pool(trx_pool& pool);
  };
}
