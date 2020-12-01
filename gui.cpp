#include "gui.h"

ATMMainWindow::ATMMainWindow(ATM* atm_machine)
{
    machine = atm_machine;
    main_widget = new MainWidget(machine);
    setCentralWidget(main_widget);
};

MainWidget::MainWidget(ATM* atm_machine){
    machine = atm_machine;
    login_screen = new LoginWidget(atm_machine);
    operator_tasks_widget = new TasksWidget(atm_machine);
    operator_tasks_widget->setup_operator_gui();
    user_tasks_widget = new TasksWidget(atm_machine);
    user_tasks_widget->setup_user_gui();
    QObject::connect(operator_tasks_widget, SIGNAL(return_to_login()), SLOT(return_to_login()));
    QObject::connect(user_tasks_widget, SIGNAL(return_to_login()), SLOT(return_to_login()));
    QObject::connect(login_screen, SIGNAL(operator_logged_in()), SLOT(operator_logged_in()));
    QObject::connect(login_screen, SIGNAL(user_logged_in()), SLOT(user_logged_in()));
    this->addWidget(login_screen);
    this->addWidget(operator_tasks_widget);
    this->addWidget(user_tasks_widget);
};

void MainWidget::operator_logged_in(){
    setCurrentWidget(operator_tasks_widget);
};

void MainWidget::user_logged_in(){
    setCurrentWidget(user_tasks_widget);
};

void MainWidget::return_to_login(){
    machine->log_out();
    login_screen->clear_pw();
    setCurrentWidget(login_screen);
}

LoginWidget::LoginWidget(ATM* atm_machine){
    machine = atm_machine;
    welcome = new QLabel;
    edit_username = new QLineEdit;
    edit_password = new QLineEdit;
    button_operator_login = new QPushButton;
    button_user_login = new QPushButton;
    main_layout = new QVBoxLayout;
    form_layout = new QFormLayout;
    button_layout = new QHBoxLayout;
    setup_gui();
};

void LoginWidget::setup_gui(){
        button_operator_login->setText("Login as an operator");
        button_user_login->setText("Login as an user");
        QObject::connect(button_operator_login, SIGNAL(clicked()), SLOT(operator_login()));
        QObject::connect(button_user_login, SIGNAL(clicked()), SLOT(user_login()));
        button_layout->addWidget(button_operator_login);
        button_layout->addWidget(button_user_login);

        form_layout->addRow("Username:", edit_username);
        edit_password->setEchoMode(QLineEdit::Password);
        form_layout->addRow("Password:", edit_password);

        welcome->setText("Please log in!");
        main_layout->addWidget(welcome);
        main_layout->addLayout(form_layout);
        main_layout->addLayout(button_layout);

        this->setLayout(main_layout);
};

void LoginWidget::operator_login(){
    if(machine->is_user_operator(edit_username->text().toInt())){
        bool logged_in = machine->log_in(edit_username->text().toInt(), edit_password->text().toStdString());
        if(logged_in){
            emit operator_logged_in();
            return;
        }
    }
    QMessageBox msgBox;
    msgBox.setText("Operator login failed");
    msgBox.exec();
    return;
};

void LoginWidget::user_login(){
    bool logged_in = machine->log_in(edit_username->text().toInt(), edit_password->text().toStdString());
    if(logged_in){
        emit user_logged_in();
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("User login failed!");
        msgBox.exec();
        return;
    }
};

void LoginWidget::clear_pw(){
    edit_password->clear();
}

TasksWidget::TasksWidget(ATM* atm_machine){
     machine = atm_machine;
     button_start_atm = new QPushButton("Start ATM");
     button_stop_atm = new QPushButton("Stop ATM");
     button_refill_atm = new QPushButton("Refill ATM");
     button_log_out = new QPushButton("Log out");
     button_withdraw = new QPushButton("Withdraw money");
     button_pay = new QPushButton("Pay bills");
     button_transfer = new QPushButton("Transfer money");
     button_check = new QPushButton("Check balance");
     v_layout = new QVBoxLayout;
}

void TasksWidget::msg(std::string msg){
    QMessageBox msgBox;
    msgBox.setText(QString::fromStdString(msg));
    msgBox.exec();
}

void TasksWidget::setup_operator_gui(){
     v_layout->addWidget(button_start_atm);
     v_layout->addWidget(button_stop_atm);
     v_layout->addWidget(button_refill_atm);
     v_layout->addWidget(button_log_out);
     QObject::connect(button_start_atm, SIGNAL(clicked()), SLOT(start()));
     QObject::connect(button_stop_atm, SIGNAL(clicked()), SLOT(stop()));
     QObject::connect(button_refill_atm, SIGNAL(clicked()), SLOT(refill()));
     QObject::connect(button_log_out, SIGNAL(clicked()), SLOT(log_out()));
     setLayout(v_layout);
};

void TasksWidget::setup_user_gui(){
    v_layout->addWidget(button_check);
    v_layout->addWidget(button_withdraw);
    v_layout->addWidget(button_transfer);
    v_layout->addWidget(button_pay);
    v_layout->addWidget(button_log_out);
    QObject::connect(button_check, SIGNAL(clicked()), SLOT(check()));
    QObject::connect(button_withdraw, SIGNAL(clicked()), SLOT(withdraw()));
    QObject::connect(button_transfer, SIGNAL(clicked()), SLOT(transfer()));
    QObject::connect(button_pay, SIGNAL(clicked()), SLOT(pay()));
    QObject::connect(button_log_out, SIGNAL(clicked()), SLOT(log_out()));
    setLayout(v_layout);
};

void TasksWidget::start(){
    machine->start();
    msg("ATM started.");
    emit return_to_login();
}

void TasksWidget::stop(){
    machine->stop();
    msg("ATM stopped.");
    emit return_to_login();
}

void TasksWidget::refill(){
    if(machine->is_started()){
        bool ok;
        int amount = QInputDialog::getInt(this,
                                            "Refill ATM",
                                            "Cash amount",
                                              0,0,10000,1,&ok);
        if(ok){
            machine->refill_cash(amount);
            msg("ATM refilled!");
            emit return_to_login();
            return;
        }
    }
    msg("Start the ATM before doing anything!");
}

void TasksWidget::log_out(){
    machine->log_out();
    msg("Logged out.");
    emit return_to_login();
}

void TasksWidget::check(){
    if(machine->is_started()){
        int balance;
        if(machine->check_balance(balance))
            msg("Your balance is " + std::to_string(balance));
        else
            msg("Operation failed");
        return;
    }
    else
        msg("ATM is not started!");
    emit return_to_login();
}

void TasksWidget::pay(){
    if(machine->is_started()){
        bool ok;
        int amount = QInputDialog::getInt(this,
                                            "Pay some bills",
                                            "Price to pay",
                                              0,0,10000,1,&ok);
        if(ok){
            if(machine->pay(amount))
                msg("You paid. :( ");
            else
                msg("You don't have enoough money. :( ");
        }
    }
    else
        msg("ATM is not started!");
    emit return_to_login();
}

void TasksWidget::transfer(){
     if(machine->is_started()){
         QDialog* d = new QDialog;
         QVBoxLayout* vbox = new QVBoxLayout;
         QFormLayout* form = new QFormLayout;
         QLineEdit* to = new QLineEdit;
         QLineEdit* amount = new QLineEdit;
         QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                             | QDialogButtonBox::Cancel);
         QObject::connect(buttonBox, SIGNAL(accepted()), d, SLOT(accept()));
         QObject::connect(buttonBox, SIGNAL(rejected()), d, SLOT(reject()));

         form->addRow("Send to (user id):", to);
         form->addRow("Amount:", amount);

         vbox->addLayout(form);
         vbox->addWidget(buttonBox);
         d->setLayout(vbox);
         int result = d->exec();
         if(result == QDialog::Accepted)
         {
             if(machine->transfer(to->text().toInt(), amount->text().toInt()))
                 msg("Money transferred.");
             else
                 msg("Operation failed.");
         }
         else
             msg("Operation cancelled.");
     }
     else
         msg("ATM is not started!");
     emit return_to_login();
}

void TasksWidget::withdraw(){
    if(machine->is_started()){
        bool ok;
        int amount = QInputDialog::getInt(this,
                                            "Withdraw",
                                            "Cash amount",
                                              0,0,10000,1,&ok);
        if(ok){
            if(machine->withdraw(amount))
                msg("You have cash now!");
            else
                msg("You or the ATM don't have enoough money. :( ");
        }
    }
    else
        msg("ATM is not started!");
    emit return_to_login();
}

