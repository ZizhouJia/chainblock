#pragma once
#include<string>
#include<vector>

namespace trx{
  struct signer{
    std::string public_key;
    std::string personal_content;
    std::string sign_time;
    std::string random_number;
    std::string sign;
    std::vector<std::string> link_trx;
  };
  class transaction{
  private:
    std::string prev_trx;
    std::string chain_name;
    std::string contract;
    std::string action;
    std::string content;
    std::vector<std::string> parties;
    std::vector<signer> signers;
    std::string trans_content_hash;

    int get_index_by_public_key(const std::string& public_key);
    void calculate_trans_content_hash();
    std::string get_signer_hash(const int index);
    void set_sign_random_number(const int index);

  public:
    void add_singer(const signer &s);

    void add_signer(const std::string& public_key,const std::string& personal_content,const std::vector<std::string>& link_trx);

    void add_parties(const std::string& partie);

    std::vector<signer> get_signers();

    std::vector<std::string> get_parties();

    void set_prev_trx(const std::string& prev_trx);

    std::string get_prev_trx();

    void set_chain_name(std::string chain_name);

    std::string get_chain_name();

    void set_contract(std::string contract);

    std::string get_contract();

    void set_action(std::string action);

    std::string get_action();

    void set_content(std::string content);

    std::string get_content();

    transaction():trans_content_hash(""){};

    transaction(std::string& json);

    std::string calculate_sign(const std::string& public_key,std::string& private_key);

    int sign_trx(const std::string& public_key,std::string& private_key);

    bool verify_trx();

    std::string to_hash();

    std::string to_json();

  };

}
