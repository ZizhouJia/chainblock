#include<string>
#include<vector>
#include<iostream>
#include "crypto/trx.h"
#include "crypto/ecc.h"
#include "block/merkle_tree.h"

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

int main(){
  test_hash_trans();
  test_merkle_tree();
}
