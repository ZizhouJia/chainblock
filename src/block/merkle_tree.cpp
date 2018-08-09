#include<vector>
#include<string>
#include "crypto/trx.h"
#include "crypto/hash.h"
#include "block/merkle_tree.h"

namespace merkle{
  merkle_tree::merkle_tree(const std::vector<std::string>& hashs){
    if(hashs.size()==0){
      this->hashs_number=0;
      this->elements_size=0;
    }
    else if(hashs.size()==1){
      this->hashs_number=1;
      this->elements_size=1;
      this->elements=new std::string[this->elements_size];
      this->elements[0]=hashs[0];
    }else{
      this->hashs_number=hashs.size();
      this->elements_size=1;
      int numbers=hashs.size();
      while(numbers!=1){
        if(numbers%2==0){
          this->elements_size+=numbers;
          numbers=numbers/2;
        }else{
          this->elements_size+=(numbers+1);
          numbers=(numbers+1)/2;
        }
      }
      this->elements=new std::string[this->elements_size];
      int current=this->elements_size-1;
      if(hashs.size()%2==1){
        this->elements[current]=hashs[hashs.size()-1];
        current--;
      }
      for(int i=hashs.size()-1;i>=0;i--){
        this->elements[current]=hashs[i];
        current--;
      }
      int index2=this->elements_size-1;
      numbers=(this->elements_size-current-1)/2;
      while(numbers!=1){
        if(numbers%2==1){
          this->elements[current]=crypto::sha256(this->elements[index2-1]+this->elements[index2]);
          current--;
        }
        for(int i=0;i<numbers;i++){
          this->elements[current]=crypto::sha256(this->elements[index2-1-i*2]+this->elements[index2-i*2]);
          current--;
        }
        index2-=numbers*2;
        numbers=(numbers+1)/2;
      }
      this->elements[0]=crypto::sha256(this->elements[1]+this->elements[2]);
    }
  }

  std::string merkle_tree::get_root(){
    if(this->elements_size==0){
      return crypto::hash0();
    }
    return elements[0];
  }

  std::vector<std::string> merkle_tree::get_hash_verify_list_by_index(const int index){
    std::vector<std::string> verify_list;
    if(index<0 || index>=this->hashs_number){
      std::cout<<"the index is out of bound"<<std::endl;
      return verify_list;
    }
    if(this->hashs_number==1){
      verify_list.push_back(this->elements[0]);
      return verify_list;
    }

    std::vector<int> nodes_per_layer;
    int numbers=this->hashs_number;
    while(numbers!=1){
      if(numbers%2==1){
        nodes_per_layer.push_back(numbers+1);
      }else{
        nodes_per_layer.push_back(numbers);
      }
      numbers=(numbers+1)/2;
    }
    int current_base=this->elements_size;
    int current_index=index;
    for(int i=0;i<nodes_per_layer.size();i++){
      current_base-=nodes_per_layer[i];
      if(i==0){
        if(current_index%2==0){
          verify_list.push_back(this->elements[current_base+current_index]);
          verify_list.push_back(this->elements[current_base+current_index+1]);
        }else{
          verify_list.push_back(this->elements[current_base+current_index-1]);
          verify_list.push_back(this->elements[current_base+current_index]);
        }
      }else{
        if(current_index%2==0){
          verify_list.push_back(this->elements[current_base+current_index+1]);
        }else{
          verify_list.push_back("r");
          verify_list.push_back(this->elements[current_base+current_index-1]);
        }
      }
      current_index=(current_index+1)/2;
    }
    return verify_list;
  }

   bool merkle_tree::verify_the_hash(const std::vector<std::string>& hash_list, std::string root_hash){
    if(hash_list.size()==0){
      std::cout<<"error in making hash_list"<<std::endl;
      return false;
    }
    if(hash_list.size()==1){
      return hash_list[0]==root_hash;
    }
    std::string current=crypto::sha256(hash_list[0]+hash_list[1]);
    int i=2;
    while(i<hash_list.size()){
      if(hash_list[i]=="r"){
        current=crypto::sha256(hash_list[i+1]+current);
        i+=2;
      }else{
        current=crypto::sha256(current+hash_list[i]);
        i++;
      }
    }
    return current==root_hash;
  }

}
