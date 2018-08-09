#include "block/merkle_tree.h"

namespace block{
  class block_lite{
    std::string version;
    std::string chain_name;
    int block_id;
    int nonce_number;
    std::string prev_hash;
    std::string time;
    std::string hash_content_root;
    std::string hash_exp_root;
    block_lite(){};
    block_lite(std::string chain_name,int block_id,std::string prev_hash.std::string time):
    chain_name(chain_name),block_id(block_id),perv_hash(prev_hash),time(time){};
  }
}
