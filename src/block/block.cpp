#include<string>
#include<memory>
#include "crypto/hash.h"
#include "crypto/trx.h"
#include "block/block.h"
#include "block/trx_pool.h"

namespace block{
  void block::set_block_from_pool(trx_pool& pool){
    this->exps=pool.get_exps();
    this->items=pool.get_trxs();
    merkle::merkle_tree* ptr=pool.make_merkle_tree();
    this->mtree=std::shared_ptr<merkle::merkle_tree>(ptr);
    this->hash_content_root=pool.get_hash_content_root();
    this->hash_exp_root=pool.get_hash_exp_root();
  };
}
