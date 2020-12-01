#ifndef ATM_H
#define ATM_H

#include <string>

class ATM
{
    public:
        bool is_user_operator(int user_id);
        bool log_in(int user_id, std::string pw);
        bool log_out();
        bool refill_cash(int amount);
        bool start();
        bool stop();
        bool is_started();
        bool withdraw(int amount);
        bool pay(int amount);
        bool transfer(int to_user_id, int amount);
        bool check_balance(int &balance);
    private:
        bool started = false;
        int cash = 0;
        int logged_in_user = -1;
};

#endif // ATM_H
