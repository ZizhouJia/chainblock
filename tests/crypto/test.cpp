#include "crypto/hash.h"
#include "crypto/utils.h"
#include "crypto/ecc.h"
#include "crypto/trx.h"
#include<string>
#include<iostream>
#include<vector>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"


void test_hash(){
  std::cout<<"begin test hash"<<std::endl;
  std::string content="123456788";
  std::cout<<content<<std::endl;
  std::string hash=crypto::sha256(content);
  std::cout<<hash<<std::endl;
  std::string str=crypto::bin2str(hash);
  std::cout<<str<<std::endl;
  std::string hash2=crypto::str2bin(str);
  std::cout<<hash2<<std::endl;
  std::string hash3=crypto::str2bin("c3160e3801345cd7b525e1cc15fd4c2ac502697a");
  std::cout<<hash3<<std::endl;
}

void test_ecc(){
  std::cout<<"begin test ecc"<<std::endl;
  //generate private key and print
  std::string private_key=crypto::generate_private_key();
  std::string private_key_str=crypto::bin2str(private_key);
  std::cout<<private_key_str.length()<<std::endl;
  std::cout<<private_key_str<<std::endl;
  // generate public key and print
  std::string public_key=crypto::generate_public_key(private_key);
  std::string public_key_str=crypto::bin2str(public_key);
  std::cout<<public_key_str.length()<<std::endl;
  std::cout<<public_key_str<<std::endl;
  // generate new  private_key and public_key
  std::string private_key2=crypto::generate_private_key();
  std::string public_key2=crypto::generate_public_key(private_key2);
  //hash the content and make sign and verify
  std::string content="Hello world";
  std::string hash=crypto::sha256(content);
  std::string sign1=crypto::sign(private_key,hash);
  std::string sign2=crypto::sign(private_key2,hash);
  if(crypto::verify(public_key,hash,sign1)){
    std::cout<<"succeed in verify"<<std::endl;
  }
  if(!crypto::verify(public_key,hash,sign2)){
    std::cout<<"succeed in unverify"<<std::endl;
  }
}

void test_trx(){
  std::cout<<"begin test trx"<<std::endl;
  std::vector<std::string> private_keys;
  std::vector<std::string> public_keys;
  std::vector<std::string> personal_contents;

  for(int i=0;i<3;i++){
    std::string private_key=crypto::generate_private_key();
    std::string public_key=crypto::generate_public_key(private_key);
    private_keys.push_back(private_key);
    public_keys.push_back(public_key);
    std::string personal_content="Hello kite"+tools::rand_number();
    personal_contents.push_back(personal_content);
  }
  std::string content="Hello world 你好!";
  trx::transaction trans(content,public_keys);

  for(int i=0;i<3;i++){
    trx::sign_trx(trans,personal_contents[i],public_keys[i],private_keys[i]);
  }

  if(trx::verify_trx(trans)){
    std::cout<<"verify succeed"<<std::endl;
  }

  std::string json=trx::trx2json(trans);

  std::cout<<json<<std::endl;

  trx::transaction trans2=trx::json2trx(json);

  if(trx::verify_trx(trans2)){
    std::cout<<" json<->trx verify succeed"<<std::endl;
  }

  trans.sign_infos[2].sign="123456";
  if(!trx::verify_trx(trans)){
    std::cout<<"unverify succeed"<<std::endl;
  }

}


int main(){
  test_hash();
  test_ecc();
  test_trx();
}
