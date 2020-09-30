#include "wsserver.h"
#include "xml2json.hpp"
#include "iostream"
using namespace std;
//Обработка нового соединения
void wsServer::newConnection()
{
    qDebug() << "New Connection";
     cout <<  "New connection: " <<  endl;
    QWebSocket *client = _server->nextPendingConnection();
    connect(client, &QWebSocket::textMessageReceived, this, &wsServer::readMessage);
    connect(client, &QWebSocket::disconnected, this, &wsServer::disconnected);
}
//Метод чтения сообщения от клиента
void wsServer::readMessage(QString message)
{
    auto * client = (QWebSocket*)(sender());
    JSON data;
    qDebug() << "New message arrived: " << message;
    cout <<  "New message arrived: " << message.toStdString() << endl;
    //Пытаемся перевести сообщение в JSON
    try
    {
        data = JSON::parse(message.toStdString());

    }
    catch (JSON::exception & e) {
        qWarning() << "Invalid json!" << message;
        cout <<  "Invalid json!" << message.toStdString() << endl;
        return;
    }

    //Проверка есть ли модуль path
    QString path;
    try
    {
        path = QString::fromStdString( data["path"]);
    }
    catch (JSON::exception & e) {
        qWarning() << "Invalid path param!" << message;
        cout <<  "Invalid path param!" << message.toStdString() << endl;
        return;
    }
    //Если все ок -  читаем файлы из пути, и конвертируем
    auto jsonList =  getConvertedXMLfromFolder(path);
    //Затем отправляем полученный результат клиенту
    for(auto & i:jsonList)
    {
        client->sendTextMessage(i);
    }



    qDebug() << message;
}

void wsServer::disconnected()
{
    auto * client = (QWebSocket*)(sender());
    client->deleteLater();
}

QStringList wsServer::getConvertedXMLfromFolder(QString path)
{
    QStringList result;
    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setNameFilters({"*.xml"});
    //Получение списка
    QFileInfoList list = dir.entryInfoList();
    for(auto i: list)
    {

        QFile file(i.filePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qWarning() << "Cant Open file " << i.filePath();
            cout <<  "Cant Open file" << i.filePath().toStdString() << endl;
            continue;
        }
        else
        {
            QByteArray data = file.readAll();
            std::string output;
            //Попытка конвертации, если все ок - отправка.
            try {
                output = xml2json(data.toStdString().c_str());
            } catch (...) {

                qWarning() << "Incorrect XML file:" << i.filePath();
                cout <<  "Incorrect XML file:" << i.filePath().toStdString() << endl;
                continue;
            }
            result.push_back(QString::fromStdString(output));
        }
    }
    return  result;
}

wsServer::wsServer(QObject *parent) : QObject(parent) {
    _server =   new QWebSocketServer("Server",QWebSocketServer::NonSecureMode,this);
    if (_server->listen(QHostAddress::Any, _port)) {
        qDebug() << "start listen...";
        cout <<  "start listen..." << endl;
        connect(_server, &QWebSocketServer::newConnection,
                this, &wsServer::newConnection);

    }


}

wsServer::~wsServer()
{
    _server->disconnect();
}


