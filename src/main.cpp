#include <QCoreApplication>
#include "file_receiver.h"
#include "file_sender.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    File_Receiver fr = File_Receiver("8000");

    File_Sender fs = File_Sender("E:\\ST\\视频\\hanser献唱《樱花，我想见你》_哔哩哔哩_bilibili.mp4","192.168.10.51","8000");

    qDebug()<<"main end";

    return a.exec();
}
