#include "file_receiver.h"

File_Receiver::File_Receiver(QString port)
{
    connect(&server, &QTcpServer::newConnection, this, &File_Receiver::connected);

    //启动服务器
    server.listen(QHostAddress::Any,port.toUShort());
}

void File_Receiver::connected()
{
    //创建与客户端通信的套接字
    msocket = server.nextPendingConnection();
    //关联读数据信号readyRead
    connect(msocket, &QTcpSocket::readyRead, this, &File_Receiver::read_data);
    filesize = 0;
    recvsize = 0;
}

void File_Receiver::read_data()
{
    if(filesize == 0) //表达第一次读取数据--读文件信息
    {
        QByteArray array = msocket->readAll();
        QDataStream stream(&array, QIODevice::ReadOnly);//把套接字与数据流绑定
        stream>>filesize>>filename;//获取文件大小， 文件名

        //打开文件
        file.setFileName(filename);
        file.open(QIODevice::WriteOnly);

    }

    //读文件内容
    if(recvsize <  filesize)
    {
        //读取一段写一段
        QByteArray array = msocket->readAll();
        file.write(array);
        recvsize += array.size();

        qDebug()<<"接收进度:"+QString::number((float)recvsize/filesize);
    }

    if(recvsize == filesize)
    {
        qDebug()<<"receive end";
        //读完，关闭文件
        file.close();
        //关闭套接字
        msocket->disconnectFromHost();
        exit(0);

    }
}
