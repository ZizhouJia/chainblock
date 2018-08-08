#include<string>
#include<iostream>
#include<openssl/rand.h>
#include<secp256k1.h>
#include "crypto/utils.h"


namespace crypto{
  static secp256k1_context* ctx_sign=secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
  static secp256k1_context* ctx_verify=secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);

  std::string generate_private_key(){
    unsigned char buf[32];
    std::string rand_number=tools::rand_number();
    RAND_seed(rand_number.c_str(),32);
    if(RAND_status()){
      RAND_bytes(buf, 32);
      std::string ret(reinterpret_cast<char*>(buf),32);
      return ret;
    }else{
      std::cout<<"rand_seed is not rand enough"<<std::endl;
      return "";
    }
  }


  std::string generate_public_key(const std::string& private_key){
    secp256k1_pubkey pubkey;
    unsigned char* priv_key=tools::cast_uchar(private_key.c_str());
    if(!secp256k1_ec_pubkey_create(ctx_sign,&pubkey, priv_key)){
      std::cout<<"error in verify private_key"<<std::endl;
      return "";
    }
    unsigned char buf[1024];
    size_t l=65;
    size_t* len=&l;
    secp256k1_ec_pubkey_serialize(ctx_sign,buf,len,&pubkey,SECP256K1_EC_UNCOMPRESSED);
    std::string public_key(reinterpret_cast<char*>(buf),*len);
    return public_key;
  }

  std::string sign(const std::string& private_key,const std::string& hash){
    //create the sign
    secp256k1_ecdsa_signature sign_c;
    //get the hash
    unsigned char* hash_c=tools::cast_uchar(hash.c_str());
    //get the private ec_key
    unsigned char* private_key_c=tools::cast_uchar(private_key.c_str());
    if(!secp256k1_ecdsa_sign(ctx_sign,&sign_c,hash_c,private_key_c,nullptr,nullptr)){
      std::cout<<"fail to sign the hash"<<std::endl;
      return "";
    }
    //make the sign into string
    unsigned char output[64];
    secp256k1_ecdsa_signature_serialize_compact(ctx_sign,output,&sign_c);
    std::string sign(reinterpret_cast<char*>(output),64);
    return sign;
  }


  bool verify(const std::string& public_key,const std::string& hash,const std::string& sign){
    //get the public key string
    unsigned char* pub_key_c=tools::cast_uchar(public_key.c_str());
    //get the public key object
    secp256k1_pubkey pubkey;
    int ret=secp256k1_ec_pubkey_parse(ctx_sign,&pubkey,pub_key_c,public_key.length());
    //get the hash string
    unsigned char* hash_c=tools::cast_uchar(hash.c_str());
    //get the sign string
    unsigned char* sign_c=tools::cast_uchar(sign.c_str());
    //get the sign object
    secp256k1_ecdsa_signature sign_obj;
    if(!secp256k1_ecdsa_signature_parse_compact(ctx_sign,&sign_obj,sign_c)){
      std::cout<<"fail to parse the sign"<<std::endl;
      return "";
    }
    int ver=secp256k1_ecdsa_verify(ctx_verify,&sign_obj,hash_c,&pubkey);
    if(ver){
      return true;
    }
    return false;
  }


}
