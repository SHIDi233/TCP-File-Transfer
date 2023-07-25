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

    //当客户端连接成功会发送connected信号， 当客户端掉线会发送disconnected信号
    connect(&socket, &QTcpSocket::connected, this, &File_Sender::send_header);

    //当套接字发送完毕会发送一个信号bytesWritten
    connect(&socket, &QTcpSocket::bytesWritten, this, &File_Sender::send_file);
}

//发送文件头信息
void File_Sender::send_header()
{
    //发送文件名，文件大小
    QFileInfo info(name);//文件信息对象
    filename = info.fileName();
    filesize = info.size();

    QByteArray array; //空间
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
        QByteArray array = file.read(1024*10);//读取一段内容
        socket.write(array);//发送一段内容
        sendsize += array.size();

        qDebug()<<"发送进度:"+QString::number((float)sendsize/filesize);
    }
    if(sendsize == filesize)
    {
        file.close();//关闭文件
    }
}
