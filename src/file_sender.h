#ifndef FILE_SENDER_H
#define FILE_SENDER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QFileInfo>
#include <QDataStream>
#include <QThread>

class File_Sender : public QThread
{
    Q_OBJECT
public:
    File_Sender(QString name, QString ip, QString port);

    File_Sender(QString name, QString ip, QString port, int rate);

private:
    QTcpSocket socket;
    QFile file;
    QString filename;
    quint64 filesize;
    quint64 sendsize;
    QString name;
    QString ip;
    QString port;

    int rate=10;

private slots:
    void send_header();
    void send_file();
};

#endif // FILE_SENDER_H
