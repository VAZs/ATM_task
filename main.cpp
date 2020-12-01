#include "gui.h"
#include "atm.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ATM* machine = new ATM;
    ATMMainWindow w(machine);
    w.show();
    return a.exec();
}
