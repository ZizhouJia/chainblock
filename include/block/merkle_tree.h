#pragma once
#include<vector>
#include<string>
#include<memory>
#include<iostream>
#include "crypto/trx.h"
#include "crypto/utils.h"
#include "crypto/hash.h"
#include "string.h"

namespace merkle{

  class merkle_tree{
  private:
    std::string* elements=nullptr;
    int hashs_number;
    int elements_size;
  public:
    merkle_tree(const std::vector<std::string>& hashs);
    merkle_tree();
    merkle_tree(const merkle_tree& copy):elements(copy.elements),hashs_number(copy.hashs_number),elements_size(copy.elements_size){
      elements=new std::string[elements_size];
      memcpy(elements,copy.elements,elements_size*sizeof(std::string));
    }

    merkle_tree operator=(merkle_tree& mtree);



    ~merkle_tree(){
      if(elements!=nullptr){
        delete[] elements;
      }
    }

    std::string get_root();
    std::vector<std::string> get_hash_verify_list_by_index(const int index);
    static bool verify_the_hash(const std::vector<std::string>& hash_list,std::string root_hash);
  };

}
