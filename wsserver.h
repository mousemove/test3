#ifndef WSSERVER_H
#define WSSERVER_H

#include <QObject>
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include <QJsonDocument>
#include "json.hpp"

#include <QDir>
using JSON = nlohmann::json;
class wsServer : public QObject
{
    Q_OBJECT
private:
    QWebSocketServer * _server;
    QList<QWebSocket *> _clients;
    unsigned _port = 5577;
private slots:
    void newConnection();
    void readMessage(QString message);
    void disconnected();
    QStringList getConvertedXMLfromFolder(QString path);
public:

    explicit wsServer(QObject *parent = nullptr);
~wsServer();

signals:

};

#endif // WSSERVER_H
