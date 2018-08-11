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
  std::string content="Hello World";
  std::vector<std::string> private_keys;
  std::vector<std::string> public_keys;

  for(int i=0;i<10;i++){
    std::string private_key=crypto::generate_private_key();
    std::string public_key=crypto::generate_public_key(private_key);
    private_keys.push_back(private_key);
    public_keys.push_back(public_key);
  }
  trx::transaction trans(content,public_keys);
  for(int i=0;i<10;i++){
    trx::sign_trx(trans,"123",public_keys[i],private_keys[i]);
  }
  if(!trx::verify_trx(trans)){
    std::cout<<"fail to verify"<<std::endl;
  }
  std::string hash1=merkle::trx2hash(trans);
  std::string json=trx::trx2json(trans);
  trx::transaction trans2=trx::json2trx(json);
  std::string hash2=merkle::trx2hash(trans2);
  if(hash1==hash2){
    std::cout<<"trx hash succeed"<<std::endl;
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
    std::string content="Hello World";
    std::vector<std::string> private_keys;
    std::vector<std::string> public_keys;
    for(int i=0;i<10;i++){
      std::string private_key=crypto::generate_private_key();
      std::string public_key=crypto::generate_public_key(private_key);
      private_keys.push_back(private_key);
      public_keys.push_back(public_key);
    }
    trx::transaction trans(content,public_keys);
    for(int i=0;i<10;i++){
      trx::sign_trx(trans,"123",public_keys[i],private_keys[i]);
    }
    if(!trx::verify_trx(trans)){
      std::cout<<"fail to verify"<<std::endl;
    }
    vector_hash.push_back(merkle::trx2hash(trans));
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
    std::string content="Hello World";
    std::vector<std::string> private_keys;
    std::vector<std::string> public_keys;
    for(int i=0;i<10;i++){
      std::string private_key=crypto::generate_private_key();
      std::string public_key=crypto::generate_public_key(private_key);
      private_keys.push_back(private_key);
      public_keys.push_back(public_key);
    }
    trx::transaction trans(content,public_keys);
    for(int i=0;i<10;i++){
      trx::sign_trx(trans,"123",public_keys[i],private_keys[i]);
    }
    if(!trx::verify_trx(trans)){
      std::cout<<"fail to verify"<<std::endl;
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

}

int main(){
  test_hash_trans();
  test_merkle_tree();
  test_pool_and_block();
}
