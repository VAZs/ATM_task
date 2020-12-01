#ifndef ATMMAINWINDOW_H
#define ATMMAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QStackedWidget>
#include <atm.h>
#include <QObject>
#include <QMessageBox>
#include <QInputDialog>
#include <string>
#include <QDialogButtonBox>

class LoginWidget: public QWidget{
    Q_OBJECT
    QLabel* welcome;
    QLineEdit* edit_username;
    QLineEdit* edit_password;
    QPushButton* button_operator_login;
    QPushButton* button_user_login;
    QVBoxLayout* main_layout;
    QFormLayout* form_layout;
    QHBoxLayout* button_layout;
    void setup_gui();
private slots:
    void operator_login();
    void user_login();
public:
    LoginWidget(ATM* atm_machine);
    ATM* machine;
    void clear_pw();
signals:
    void operator_logged_in();
    void user_logged_in();
};

class TasksWidget: public QWidget
{
    Q_OBJECT
    QVBoxLayout* v_layout;
    QPushButton* button_start_atm;
    QPushButton* button_stop_atm;
    QPushButton* button_refill_atm;
    QPushButton* button_log_out;
    QPushButton* button_withdraw;
    QPushButton* button_pay;
    QPushButton* button_transfer;
    QPushButton* button_check;
    ATM* machine;

public:
    TasksWidget(ATM* atm_machine);
    void setup_operator_gui();
    void setup_user_gui();
    void msg(std::string msg);
private slots:    
    void start();
    void stop();
    void refill();
    void log_out();
    void check();
    void pay();
    void transfer();
    void withdraw();

signals:
    void return_to_login();
};

class MainWidget: public QStackedWidget{
public:
    MainWidget(ATM* atm_machine);
private:
    Q_OBJECT
    LoginWidget* login_screen;
    TasksWidget* operator_tasks_widget;
    TasksWidget* user_tasks_widget;
    void setup_gui();
    ATM* machine;
private slots:
    void operator_logged_in();
    void user_logged_in();
    void return_to_login();
};

class ATMMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    ATMMainWindow(ATM* atm_machine);
    MainWidget* main_widget;
    ATM* machine;
};

#endif // ATMMAINWINDOW_H
