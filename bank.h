#ifndef DATABASE_H
#define DATABASE_H

#include<string>
#include<map>
#include<vector>
#include<algorithm>

class Person{
  public:
    std::string name;
    int ID;
    std::string password;
};

class Bank{
  public:
    bool query_balance(int user_id, int& balance);
    bool query_password(int user_id, std::string& pw);
    bool is_person_operator(int user_id);
    bool transfer(int from_user_id, int to_user_id, int amount);
    bool deduct(int user_id, int amount);
    static Bank* get_bank();
    Bank(Bank&) = delete;
    void operator=(const Bank&) = delete;
  private:
    Bank();
    static Bank* only_one_bank;
    std::map<int, Person> persons;
    std::map<int, int> userid_to_money;
    std::vector<int> operators;
};

#endif // DATABASE_H
