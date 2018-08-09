#include<vector>
#include<string>
#include "crypto/trx.h"
namespace block{
  class block{
  private:
    vector<trx::transaction> items;
    vector<std::string> exps;
    block_lite blit;
  public:
    block(std::string chain_name,int block_id,std::string prev_hash,std::string block_time):blit(chain_name,block_id,prev_hash,time){};
    void add_trx(trx::transaction trans,std::string exp);
    void calculate_hash_content_root();
    void calculate_hash_exp_root();
  }
}
