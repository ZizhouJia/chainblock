#include<vector>
#include<string>
#include "crypto/trx.h"
#include "crypto/utils.h"
#include "crypto/hash.h"
namespace merkle{
  class merkle_node{
  public:
    merkle_node* left;
    merkle_node* right;
    std::string hash;
    merkle_node(std::string hash,merkle_node* left=nullptr,merkle_node* right=nullptr):left(left),right(right),hash(hash){};
    ~merkle_node(){
      if(left!=nullptr){
        delete left;
      }
      if(right!=nullptr){
        delete right;
      }
    }
  };

  class merkle_tree{
  public:
    merkle_node* root;
    merkle_tree(merkle_node* root):root(root){
    }
    ~merkle_tree(){
      if(root!=nullptr){
        delete root;
      }
    }
  };

  std::string trx2hash(trx::transaction trans){
    return crypto::sha256(trx::trx2json(trans));
  }

  merkle_tree vec2mtree(std::vector<std::string> hash);

  std::vector<std::string> mtree2vec(const merkle_tree& mtree);

  bool merkle_tree_equal(const merkle_tree& mtree1,const merkle_tree mtree2);

}
