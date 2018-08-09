#include<vector>
#include<string>
#include "crypto/trx.h"
#include "crypto/hash.h"
#include "block/merkle_tree.h"

namespace merkle{
  merkle_tree vec2mtree(std::vector<std::string> hash){
    return merkle_tree(nullptr);
  }

  std::vector<std::string> mtree2vec(const merkle_tree& mtree){
    std::vector<std::string> v;
    return v;
  }

  bool merkle_tree_equal(const merkle_tree& mtree1,const merkle_tree mtree2){
    return false;
  }

}
