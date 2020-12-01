#include "bank.h"

Bank* Bank::only_one_bank = nullptr;

bool Bank::is_person_operator(int user_id){
    return std::find(operators.begin(), operators.end(), user_id) != operators.end();
}

bool Bank::query_balance(int user_id, int &balance){
    auto search_result = userid_to_money.find(user_id);
    if(search_result == userid_to_money.end())
        return false;
    else{
        balance = search_result->second;
        return true;
    }
}

bool Bank::query_password(int user_id, std::string& pw){
    auto search_result = persons.find(user_id);
    if(search_result == persons.end())
        return false;
    else{
        pw = search_result->second.password;
        return true;
    }
}

bool Bank::deduct(int user_id, int amount){
    int balance;
    if(query_balance(user_id, balance)){
        if(balance >= amount){
            userid_to_money[user_id] -= amount;
            return true;
        }
    }
    return false;
}

bool Bank::transfer(int from_user_id, int to_user_id, int amount){
    auto search_result = persons.find(to_user_id);
    if(search_result != persons.end()){
        if(deduct(from_user_id, amount)){
            userid_to_money[to_user_id] += amount;
            return true;
        }
    }
    return false;
}

Bank::Bank(){
    Person a, b, c, d;
    a.ID = 1;
    a.name = "AAA";
    a.password = "1";

    b.ID = 2;
    b.name = "BBB";
    b.password = "2";

    c.ID = 3;
    c.name = "CCC";
    c.password = "3";

    d.ID = 4;
    d.name = "DDD";
    d.password = "4";

    persons.insert({1,a});
    persons.insert({2,b});
    persons.insert({3,c});
    persons.insert({4,d});

    operators.push_back(3);
    operators.push_back(4);

    userid_to_money.insert({1, 100});
    userid_to_money.insert({2, 200});
    userid_to_money.insert({3, 300});
    userid_to_money.insert({4, 400});
}

Bank* Bank::get_bank(){
    if(Bank::only_one_bank==nullptr){
        Bank::only_one_bank = new Bank();
    }
    return Bank::only_one_bank;
}
