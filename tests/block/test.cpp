#include<string>
#include<vector>
#include<iostream>
#include "crypto/trx.h"
#include "crypto/ecc.h"
#include "block/merkle_tree.h"
#include "block/block.h"
#include "block/trx_pool.h"
#include "crypto/utils.h"

void test_hash_trans(){
  std::cout<<"begin test transaction hash"<<std::endl;
  std::vector<std::string> private_keys;
  std::vector<std::string> public_keys;
  std::vector<std::string> personal_contents;
  trx::transaction trans;
  trans.set_chain_name("123");
  trans.set_contract("456");
  trans.set_prev_trx("678");
  trans.set_action("action");
  trans.set_content("Hello world 你好!");
  trans.add_parties("123");
  for(int i=0;i<3;i++){
    std::string private_key=crypto::generate_private_key();
    std::string public_key=crypto::generate_public_key(private_key);
    private_keys.push_back(private_key);
    public_keys.push_back(public_key);
    std::string personal_content="Hello kite"+tools::rand_number();
    std::vector<std::string> link_trx;
    for(int j=0;i<i;j++){
        link_trx.push_back("345");
    }
    trans.add_signer(public_key,personal_content,link_trx);
  }

  for(int i=0;i<3;i++){
    trans.sign_trx(public_keys[i],private_keys[i]);
  }

  std::string hash1=trans.to_hash();
  std::cout<<crypto::bin2str(hash1)<<std::endl;
  std::string json=trans.to_json();
  trx::transaction trans2(json);
  std::string hash2=trans2.to_hash();
  if(hash1==hash2){
    std::cout<<"trx hash succeed"<<std::endl;
  }else{
    std::cout<<"fail on test hash"<<std::endl;
  }
}

void test_merkle_tree(){
  std::cout<<"begin test merkle"<<std::endl;
  std::vector<std::string> vector_hash;
  merkle::merkle_tree mtree1=merkle::merkle_tree(vector_hash);
  std::cout<<crypto::bin2str(mtree1.get_root())<<std::endl;
  vector_hash.push_back("1234566778999");
  merkle::merkle_tree mtree2=merkle::merkle_tree(vector_hash);
  std::cout<<mtree2.get_root()<<std::endl;
  vector_hash.clear();
  for(int j=0;j<10;j++){
    std::vector<std::string> private_keys;
    std::vector<std::string> public_keys;
    std::vector<std::string> personal_contents;
    trx::transaction trans;
    trans.set_chain_name("123");
    trans.set_contract("456");
    trans.set_prev_trx("678");
    trans.set_action("action");
    trans.set_content("Hello world 你好!");
    trans.add_parties("123");
    for(int i=0;i<3;i++){
      std::string private_key=crypto::generate_private_key();
      std::string public_key=crypto::generate_public_key(private_key);
      private_keys.push_back(private_key);
      public_keys.push_back(public_key);
      std::string personal_content="Hello kite"+tools::rand_number();
      std::vector<std::string> link_trx;
      for(int j=0;i<i;j++){
          link_trx.push_back("345");
      }
      trans.add_signer(public_key,personal_content,link_trx);
    }

    for(int i=0;i<3;i++){
      trans.sign_trx(public_keys[i],private_keys[i]);
    }
    vector_hash.push_back(trans.to_hash());
  }
  merkle::merkle_tree mtree3=merkle::merkle_tree(vector_hash);
  std::string root_hash=mtree3.get_root();
  std::vector<std::string> verify_list=mtree3.get_hash_verify_list_by_index(0);
  if(vector_hash[0]==verify_list[0]){
    std::cout<<"verify the content succeed"<<std::endl;
  }
  if(merkle::merkle_tree::verify_the_hash(verify_list,root_hash)){
    std::cout<<"verify the hash succeed"<<std::endl;
  }
}

void test_pool_and_block(){
  block::trx_pool pool;
  for(int j=0;j<10;j++){
    std::vector<std::string> private_keys;
    std::vector<std::string> public_keys;
    std::vector<std::string> personal_contents;
    trx::transaction trans;
    trans.set_chain_name("123");
    trans.set_contract("456");
    trans.set_prev_trx("678");
    trans.set_action("action");
    trans.set_content("Hello world 你好!");
    trans.add_parties("123");
    for(int i=0;i<3;i++){
      std::string private_key=crypto::generate_private_key();
      std::string public_key=crypto::generate_public_key(private_key);
      private_keys.push_back(private_key);
      public_keys.push_back(public_key);
      std::string personal_content="Hello kite"+tools::rand_number();
      std::vector<std::string> link_trx;
      for(int j=0;i<i;j++){
          link_trx.push_back("345");
      }
      trans.add_signer(public_key,personal_content,link_trx);
    }

    for(int i=0;i<3;i++){
      trans.sign_trx(public_keys[i],private_keys[i]);
    }
    pool.add_member(trans,"exps");
  }

  std::string chain_name="root";
  int block_id=0;
  std::string prev_hash=crypto::hash0();
  std::string t=tools::get_time();
  std::cout<<"time is :"<<t<<std::endl;
  block::block b(chain_name,block_id,prev_hash,t);
  b.set_block_from_pool(pool);
  b.init_calculate();
  if(b.calculate_nonce()){
    std::cout<<"succeed on find the nonce"<<std::endl;
  }else{
    std::cout<<"fail on find the nonce"<<std::endl;
  }
  std::string hash=b.get_hash();
  std::cout<<crypto::bin2str(hash)<<std::endl;
  if(b.verify_the_block_hash()){
    std::cout<<"succeed on verify the block hash"<<std::endl;
  }else{
    std::cout<<"fail on verify the block hash"<<std::endl;
  }
  b.init_calculate();
  while(!b.calculate_nonce_once());

  if(b.verify_the_block_hash()){
    std::cout<<"succeed on verify the block hash"<<std::endl;
  }else{
    std::cout<<"fail on verify the block hash"<<std::endl;
  }


}

int main(){
  test_hash_trans();
  test_merkle_tree();
  test_pool_and_block();
}
