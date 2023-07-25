#ifndef FILE_RECEIVER_H
#define FILE_RECEIVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QDataStream>
#include <QThread>

class File_Receiver : public QThread
{
    Q_OBJECT
public:
    File_Receiver(QString port);
protected slots:
    void read_data();
    void connected();
private:
    QTcpServer server;
    QFile file;
    QString filename;
    quint64 filesize;
    quint64 recvsize;

    QTcpSocket *msocket;
};

#endif // FILE_RECEIVER_H
