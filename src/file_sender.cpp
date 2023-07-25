#include "file_sender.h"

File_Sender::File_Sender(QString name, QString ip, QString port)
{
    this->name = name;

    //连接服务器
    socket.connectToHost(ip, port.toUShort());

    //发送文件信息
    qDebug()<<"connect";

    //初始化
    filesize = 0;
    sendsize = 0;

    //连接成功会发送connected
    connect(&socket, &QTcpSocket::connected, this, &File_Sender::send_header);

    //发送完毕信号
    connect(&socket, &QTcpSocket::bytesWritten, this, &File_Sender::send_file);
}

File_Sender::File_Sender(QString name, QString ip, QString port, int rate)
{
    this->name = name;

    this->rate = rate;

    //连接服务器
    socket.connectToHost(ip, port.toUShort());

    //初始化
    filesize = 0;
    sendsize = 0;

    //连接成功信号
    connect(&socket, &QTcpSocket::connected, this, &File_Sender::send_header);

    //发送完毕信号
    connect(&socket, &QTcpSocket::bytesWritten, this, &File_Sender::send_file);
}

//发送文件头信息
void File_Sender::send_header()
{
    //发送文件名，文件大小
    QFileInfo info(name);//文件信息对象
    filename = info.fileName();
    filesize = info.size();

    QByteArray array;
    //把array与数据流绑定
    QDataStream stream(&array, QIODevice::WriteOnly);
    stream<<filesize<<filename;

    //打开文件准备读取数据发送
    file.setFileName(name);
    file.open(QIODevice::ReadOnly);

    //发送
    socket.write(array);
}

void File_Sender::send_file()
{
    if(sendsize < filesize)
    {
        QByteArray array = file.read(1024*rate);//读取
        socket.write(array);//发送
        sendsize += array.size();

        qDebug()<<"发送进度:"+QString::number((float)sendsize/filesize);
    }
    if(sendsize == filesize)
    {
        file.close();//关闭文件
    }
}
