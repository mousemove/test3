#include <QCoreApplication>
#include "wsserver.h"
#include "toolkit.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Toolkit::setPath("debug.txt","info.txt","warning.txt","critical.txt","fatal.txt");
    qInstallMessageHandler(Toolkit::messageHandler);
    wsServer server;
    return a.exec();
}
