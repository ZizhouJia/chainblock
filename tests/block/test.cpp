#include<string>
#include<vector>
#include<iostream>
#include "crypto/trx.h"
#include "crypto/ecc.h"
#include "block/merkle_tree.h"

void test_hash(){
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

  merkle::merkle_tree mtree1=merkle::vec2mtree(vector_hash);
  std::vector<std::string> vector_hash2=merkle::mtree2vec(mtree1);
  merkle::merkle_tree mtree2=merkle::vec2mtree(vector_hash2);
  if(merkle::merkle_tree_equal(mtree1,mtree2)){
    std::cout<<"merkle tree test succeed"<<std::endl;
  }else{
    std::cout<<"fail on test merkle tree test"<<std::endl;
  }

}

int main(){
  test_hash();
  test_merkle_tree();
}
