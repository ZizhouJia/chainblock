#include<string>
#include<sstream>
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<openssl/rand.h>
#include "crypto/utils.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace tools{
  unsigned char* cast_uchar(const char* str_c){
    return reinterpret_cast<unsigned char*>(
      const_cast<char*>(str_c));
  }

  std::string rand_number(){
    srand((unsigned)time(NULL));
    int number=rand();
    std::string ret;
    std::stringstream ss;
    ss << number;
    ss >> ret;
    return ret;
  }

  unsigned complex_rand_number(){
    unsigned char buf[4];
    std::string r=rand_number();
    RAND_seed(r.c_str(),32);
    if(RAND_status()){
      RAND_bytes(buf, 4);
      unsigned int number=*((unsigned int*)(buf));
      return number;
    }else{
      std::cout<<"rand_seed is not rand enough"<<std::endl;
      return 0;
    }
  }

  std::string get_time(){
    time_t timep;
    time (&timep);
    char tmp[64]={0};
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
    std::string t=tmp;
    return t;
 }
}

namespace crypto{
  int char2int(const char c){
    if(c>='0' && c<='9'){
      return c-48;
    }
    if(c>='a' && c<='f'){
      return c-87;
    }
    if(c>='A' && c<='F'){
      return c-55;
    }
    return -1;
  }

  std::string bin2str(const std::string& bin){
    std::string str;
    char buf[2];
    for(auto it=bin.begin();it!=bin.end();++it){
      sprintf(buf,"%02x",static_cast<unsigned char>(*it));
      str.append(buf);
    }
    return str;
  }

  std::string str2bin(const std::string& str){
    if(str.length()%2!=0){
      return "";
    }
    std::string bin;
    for(int i=0;i<str.length();i+=2){
      int number1=char2int(str[i]);
      int number2=char2int(str[i+1]);
      if(number1<0 || number2<0){
        return "";
      }
      char c=(char)(number1*16+number2);
      bin=bin+c;
    }
    return bin;
  }
}

namespace trx{
  // std::string trx2json(const trx::transaction& trans){
  //   rapidjson::Document jsonDoc;
  //   rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
  //   jsonDoc.SetObject();
  //   jsonDoc.AddMember("content",rapidjson::StringRef(trans.content.c_str(),trans.content.length()),allocator);
  //   jsonDoc.AddMember("prev_trx",rapidjson::StringRef(trans.prev_trx.c_str(),trans.prev_trx.length()),allocator);
  //   rapidjson::Value signs(rapidjson::kArrayType);
  //   for(int i=0;i<trans.sign_infos.size();++i){
  //     rapidjson::Value info(rapidjson::kObjectType);
  //     std::string public_key=crypto::bin2str(trans.sign_infos[i].public_key);
  //     rapidjson::Value str_val;
  //     str_val.SetString(public_key.c_str(),public_key.length(),allocator);
  //     info.AddMember("public_key",str_val,allocator);
  //     // info.AddMember("public_key",rapidjson::StringRef(public_key.c_str(),public_key.length()),allocator);
  //     info.AddMember("personal_content",rapidjson::StringRef((trans.sign_infos[i].personal_content).c_str(),
  //     (trans.sign_infos[i].personal_content).length()), allocator);
  //     info.AddMember("time",rapidjson::StringRef((trans.sign_infos[i].time).c_str(),(trans.sign_infos[i].time).length()), allocator);
  //     info.AddMember("random_number",rapidjson::StringRef((trans.sign_infos[i].random_number).c_str(),
  //     (trans.sign_infos[i].random_number).length()), allocator);
  //     std::string sign=crypto::bin2str(trans.sign_infos[i].sign);
  //     rapidjson::Value str_val_sign;
  //     str_val_sign.SetString(sign.c_str(),sign.length(),allocator);
  //     info.AddMember("sign",str_val_sign, allocator);
  //     signs.PushBack(info,allocator);
  //   }
  //   jsonDoc.AddMember("sign_infos", signs, allocator);
  //   rapidjson::StringBuffer buffer;
  //   rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  //   jsonDoc.Accept(writer);
  //   std::string strJson = buffer.GetString();
  //   return strJson;
  // }
  //
  // transaction json2trx(const std::string& json){
  //   rapidjson::Document doc;
  //   transaction empty;
  //   doc.Parse<0>(json.c_str(),json.length());
  //   if(doc.HasParseError()){
  //     std::cout<<"Json Parse Error"<<std::endl;
  //     return empty;
  //   }
  //   transaction trans;
  //   if (!doc.HasMember("content")){
  //     std::cout<<"No element content"<<std::endl;
  //     return empty;
  //   }
  //   rapidjson::Value& content=doc["content"];
  //   if(!content.IsString()){
  //     std::cout<<"Element content type error"<<std::endl;
  //     return empty;
  //   }
  //   trans.content=content.GetString();
  //
  //   if (!doc.HasMember("prev_trx")){
  //     std::cout<<"No element prev_trx"<<std::endl;
  //     return empty;
  //   }
  //   rapidjson::Value& prev_trx=doc["prev_trx"];
  //   if(!content.IsString()){
  //     std::cout<<"Element prev_trx type error"<<std::endl;
  //     return empty;
  //   }
  //   trans.prev_trx=prev_trx.GetString();
  //
  //   if (!doc.HasMember("sign_infos")){
  //     std::cout<<"No element sign_infos"<<std::endl;
  //     return empty;
  //   }
  //   rapidjson::Value& sign_infos=doc["sign_infos"];
  //   if(!sign_infos.IsArray()){
  //     std::cout<<"Element content type error"<<std::endl;
  //     return empty;
  //   }
  //
  //   for (rapidjson::SizeType i = 0; i < sign_infos.Size(); i++) {
  //     sign_info info;
  //     if (!sign_infos[i].HasMember("public_key")){
  //       std::cout<<"No element public_key"<<std::endl;
  //       return empty;
  //     }
  //     rapidjson::Value& public_key=sign_infos[i]["public_key"];
  //     if(!public_key.IsString() || public_key.GetStringLength()==0){
  //       std::cout<<"Element public_key type error"<<std::endl;
  //       return empty;
  //     }
  //
  //     if (!sign_infos[i].HasMember("personal_content")){
  //       std::cout<<"No element personal_content"<<std::endl;
  //       return empty;
  //     }
  //     rapidjson::Value& personal_content=sign_infos[i]["personal_content"];
  //     if(!personal_content.IsString()){
  //       std::cout<<"Element public_key type error"<<std::endl;
  //       return empty;
  //     }
  //
  //     if (!sign_infos[i].HasMember("time")){
  //       std::cout<<"No element time"<<std::endl;
  //       return empty;
  //     }
  //     rapidjson::Value& time=sign_infos[i]["time"];
  //     if(!time.IsString()||time.GetStringLength()==0){
  //       std::cout<<"Element time type error"<<std::endl;
  //       return empty;
  //     }
  //
  //     if (!sign_infos[i].HasMember("random_number")){
  //       std::cout<<"No element random_number"<<std::endl;
  //       return empty;
  //     }
  //     rapidjson::Value& random_number=sign_infos[i]["random_number"];
  //     if(!random_number.IsString()||random_number.GetStringLength()==0){
  //       std::cout<<"Element random_number type error"<<std::endl;
  //       return empty;
  //     }
  //
  //     if (!sign_infos[i].HasMember("sign")){
  //       std::cout<<"No element sign"<<std::endl;
  //       return empty;
  //     }
  //     rapidjson::Value& sign=sign_infos[i]["sign"];
  //     if(!sign.IsString()||sign.GetStringLength()==0){
  //       std::cout<<"Element sign type error"<<std::endl;
  //       return empty;
  //     }
  //     info.public_key=crypto::str2bin(public_key.GetString());
  //     info.random_number=random_number.GetString();
  //     info.sign=crypto::str2bin(sign.GetString());
  //     info.time=time.GetString();
  //     info.personal_content=personal_content.GetString();
  //     trans.sign_infos.push_back(info);
  //   }
  //   return trans;
  // }

}
