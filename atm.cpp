#include "atm.h"
#include "bank.h"


bool ATM::is_user_operator(int user_id){
    Bank* bank = Bank::get_bank();
    return bank->is_person_operator(user_id);
}

bool ATM::log_in(int user_id, std::string pw){
    Bank* bank = Bank::get_bank();
    std::string pw_received;
    if(bank->query_password(user_id, pw_received)){
        if(pw == pw_received){
            logged_in_user = user_id;
            return true;
        }
    }
    return false;
}

bool ATM::log_out(){
    logged_in_user = -1;
    return true;
}

bool ATM::refill_cash(int amount){
    if(started){
        Bank* bank = Bank::get_bank();
        if(bank->is_person_operator(logged_in_user)){
            cash += amount;
            return true;
        }
    }
    return false;
}

bool ATM::start(){
    Bank* bank = Bank::get_bank();
    if(bank->is_person_operator(logged_in_user)){
        started = true;
        return true;
    }
    return false;
}

bool ATM::stop(){
    Bank* bank = Bank::get_bank();
    if(bank->is_person_operator(logged_in_user)){
        started = false;
        return true;
    }
    return false;
}

bool ATM::is_started(){
    return started;
}

bool ATM::withdraw(int amount){
    if(started){
        Bank* bank = Bank::get_bank();
        if(amount <= cash && bank->deduct(logged_in_user, amount)){
            cash -= amount;
            return true;
        }
    }
    return false;
}

bool ATM::pay(int amount){
    if(started){
        Bank* bank = Bank::get_bank();
        return bank->deduct(logged_in_user, amount);
    }
    return false;
}

bool ATM::transfer(int to_user_id, int amount){
    if(started){
        Bank* bank = Bank::get_bank();
        return bank->transfer(logged_in_user, to_user_id, amount);
    }
    return false;
}

bool ATM::check_balance(int &balance){
    if(started){
        Bank* bank = Bank::get_bank();
        return bank->query_balance(logged_in_user, balance);
    }
    return false;
}
