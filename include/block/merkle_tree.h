#include<vector>
#include<string>
#include<iostream>
#include "crypto/trx.h"
#include "crypto/utils.h"
#include "crypto/hash.h"
namespace merkle{

  std::string trx2hash(trx::transaction trans){
    return crypto::sha256(trx::trx2json(trans));
  }

  class merkle_tree{
  private:
    std::string* elements=nullptr;
    int hashs_number;
    int elements_size;
  public:
    merkle_tree(const std::vector<std::string>& hashs);

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
