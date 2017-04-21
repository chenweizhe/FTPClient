#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) //ui chushihua
{
    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::Password);
}

MainWindow::~MainWindow()
{

    delete ui;
}

/*
 * 控制连接
 */
bool MainWindow::ControlConnect(){
    if((SocketControl = socket(AF_INET,SOCK_STREAM,0))<0){
       m_Logininfo = "creatsocket error:"+errno;
       qDebug("creatsocket error ");
       ui->loginInfo->append("creat socket error");
       exit(0);
    }
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(CONNECTPORT);

    /*
     *  从textedit中取出地址数据
     */
    m_Address = ui->address->text();
    if(inet_pton(AF_INET,m_Address.toStdString().data(),&serverAddr.sin_addr)<=0){
        qDebug("inet_pton error");
        exit(0);
    }

    ui->loginInfo->append("connect to server...");
    if(::connect(SocketControl,(struct sockaddr*)&serverAddr,sizeof(serverAddr))<0){
        qDebug("connect error: ");
        ui->loginInfo->append("connect error: "+errno);
        exit(0);
    }

    ui->loginInfo->append("connect success");

    if(recv(SocketControl,Respond,MAXSIZE,0)<0){
        qDebug("receive error");
        ui->loginInfo->append("receive error");
        exit(0);
    }
    ui->loginInfo->append("receive success");
    ui->loginInfo->append(Respond);
    ui->loginInfo->append("--------------------------");
    QMessageBox::information(NULL,"info","contorlConnect to ftpserver\r\n success",QMessageBox::Yes);
    return true;
}

/**
 * @brief MainWindow::FTPLogin
 * @return
 * 用户登录
 */
bool MainWindow::FTPLogin(){
    m_Username = ui->username->text();
    m_Password = ui->password->text();
    bzero(&Sendbuf,MAXSIZE);
    bzero(&Respond,MAXSIZE);
    memcpy(Sendbuf,"USER ",strlen("USER "));
    memcpy(Sendbuf+strlen("USER "),m_Username.toStdString().data(),strlen(m_Username.toStdString().data()));
    memcpy(Sendbuf+strlen("USER ")+strlen(m_Username.toStdString().data()),"\r\n",2);
    if(send(SocketControl,Sendbuf,strlen(Sendbuf),0)<0){
        qDebug("socket username send error");
        ui->loginInfo->append("socket username send error "+errno);
        exit(0);
    }

    ui->loginInfo->append("socket username send success");
    if(recv(SocketControl,Respond,MAXSIZE,0)<0){
        qDebug("socket username receive error");
         ui->loginInfo->append("socket username receive error");
    }
    ui->loginInfo->append(Respond);

    ui->loginInfo->append("password login...");
    bzero(&Sendbuf,MAXSIZE);
    bzero(&Respond,MAXSIZE);
    memcpy(Sendbuf,"PASS ",strlen("PASS "));
    memcpy(Sendbuf+strlen("PASS "),m_Password.toStdString().data(),strlen(m_Password.toStdString().data()));
    memcpy(Sendbuf+strlen("PASS ")+strlen(m_Password.toStdString().data()),"\r\n",2);
    if(send(SocketControl,Sendbuf,strlen(Sendbuf),0)<0){
        qDebug("socket password send error");
         ui->loginInfo->append("socket password send error");
    }
    ui->loginInfo->append("socket password send success ");
    if(recv(SocketControl,Respond,MAXSIZE,0)<0){
        qDebug("socket password receive error");
        ui->loginInfo->append("socket password receive error");
    }
    ui->loginInfo->append(Respond);

    return true;
}

/**
 * @brief MainWindow::DataConnect
 * @return
 * 数据连接--主动
 */
bool MainWindow::DataConnect(){
    //zhudongsocket
    if((SocketConnect = socket(AF_INET,SOCK_STREAM,0))<0){
        qDebug("creat connectsocket error");
        ui->loginInfo->append("creat connectsocket error");
        exit(0);
    }
    //shujusocket
    if((SocketData = socket(AF_INET,SOCK_STREAM,0))<0){
       m_Logininfo = "creat datasocket error:"+errno;
       qDebug("creat datasocket error ");
       ui->loginInfo->append("creat datasocket error");
       exit(0);
    }

    dataAddr.sin_family=AF_INET;
    dataAddr.sin_port = htons(p1*256+p2);
    if(inet_pton(AF_INET,m_Address.toStdString().data(),&serverAddr.sin_addr)<=0){
            qDebug("inet_pton error");
            exit(0);
     }
    //bind
    if(bind(SocketConnect,(struct sockaddr*)&dataAddr,sizeof(dataAddr))<0){
        qDebug("bind error");
        ui->loginInfo->append("bind error");
        exit(0);
    }
    ui->loginInfo->append("bind client dataAddr success");
    if(listen(SocketConnect,3) < 0){
        qDebug("listen error");
        ui->loginInfo->append("listen error");
        exit(0);
    }

    /*
     * //获取本机ip
     * char hname[128];
     * struct hostent *hent;
     * gethostname(hname, sizeof(hname));
     * hent = gethostbyname(hname);
     * hent->h_addr;
     * hp->h_length;
     *
     */
    return true;
}


bool MainWindow::PORTRequest(){
    ui->loginInfo->append("PORT....");

    //PORT Request
    bzero(Sendbuf,MAXSIZE);
    memcpy(Sendbuf,"PORT 127,0,0,1,20,80\r\n",strlen("PORT 127,0,0,1,20,80\r\n"));

    if(send(SocketControl,Sendbuf,strlen(Sendbuf),0)<0){
        qDebug("PORT Request Error");
        ui->loginInfo->append("PORT Request Error");
        exit(0);
    }
    ui->loginInfo->append(Sendbuf);
    bzero(Respond,MAXSIZE);
    if(recv(SocketControl,Respond,MAXSIZE,0)<0){
        qDebug("receive portmessage error");
        ui->loginInfo->append("receive portmessage error");
        exit(0);
    }
    ui->loginInfo->append(Respond);
    return true;
}



/**
 * @brief ::MainWindow::LISTRequest
 * @return
 */
bool::MainWindow::LISTRequest(){
    bzero(Sendbuf,MAXSIZE);
    bzero(Respond,MAXSIZE);
    struct sockaddr_in clientname;
    memcpy(Sendbuf,"LIST\r\n",strlen("LIST\r\n"));
    if(send(SocketControl,Sendbuf,strlen(Sendbuf),0)<0){
        qDebug("send List Request error");
        ui->loginInfo->append("send List Request error");
    }
    ui->loginInfo->append("send List Request success");
    if(recv(SocketControl,Respond,MAXSIZE,0)<0){
            qDebug("socket LIST receive error");
            ui->loginInfo->append("socket LIST receive error");
    }
    ui->loginInfo->append("socket LIST receive success");
    //ui->loginInfo->append(Respond);

    //acept information
    int length = sizeof(clientname);
    if((SocketData = accept(SocketConnect,(struct sockaddr*)&clientname,(socklen_t*)&length))<0){
        qDebug("accept infomation error");
        ui->loginInfo->append("accept infomation error");
    }

    ui->loginInfo->append("accept infomation success");


        bzero(Respond,MAXSIZE);
        if(recv(SocketData,Respond,MAXSIZE,0)<0){
            qDebug("receive list error");
            exit(0);
        }
        ui->direInfo->setText(Respond);

    return true;
}

/**
 * @brief MainWindow::QUITRequest
 * @return
 * 使用QUIT命令
 */
bool MainWindow::QUITRequest(){
    bzero(Sendbuf,MAXSIZE);
    bzero(Respond,MAXSIZE);
    memcpy(Sendbuf,"QUIT\r\n",strlen("QUIT\r\n"));
    if(send(SocketControl,Sendbuf,strlen(Sendbuf),0)<0){
        qDebug("quit ftp request error");
        exit(0);
    }
    ui->loginInfo->append("quit ftp send success");
    if(recv(SocketControl,Respond,MAXSIZE,0)<0){
         qDebug("quit ftp receive error");
    }
    ui->loginInfo->append("quit success");
    ui->loginInfo->append(Respond);
    if(QMessageBox::information(NULL,"info","quit ftpserver\r\n success",QMessageBox::Yes)){
        this->close();
    }
    return true;
}


/*
 *
 * download
 *
 */
bool MainWindow::RETRRequest(){
    bzero(Sendbuf,MAXSIZE);
    bzero(Sendbuf,MAXSIZE);
    struct sockaddr_in clientname;
    if(ui->filename->text() != NULL){

        //PORT Request
        bzero(Sendbuf,MAXSIZE);
        memcpy(Sendbuf,"PORT 127,0,0,1,20,80\r\n",strlen("PORT 127,0,0,1,20,80\r\n"));

        if(send(SocketControl,Sendbuf,strlen(Sendbuf),0)<0){
            qDebug("PORT Request Error");
            ui->loginInfo->append("PORT Request Error");
            exit(0);
        }
        ui->loginInfo->append(Sendbuf);
        bzero(Respond,MAXSIZE);
        if(recv(SocketControl,Respond,MAXSIZE,0)<0){
            qDebug("receive portmessage error");
            ui->loginInfo->append("receive portmessage error");
            exit(0);
        }
        ui->loginInfo->append(Respond);
        ui->loginInfo->append("-----------------------------------");


        /*************TYPE*********************/
        bzero(Sendbuf,MAXSIZE);
        bzero(Respond,MAXSIZE);
        memcpy(Sendbuf,"TYPE I\r\n",strlen("TYPE I\r\n"));
        if(send(SocketControl,Sendbuf,strlen(Sendbuf),0)<0){
            qDebug("send List Request error");
            ui->loginInfo->append("send TYPE Request error");
        }
        ui->loginInfo->append("send TYPE Request success");
        if(recv(SocketControl,Respond,MAXSIZE,0)<0){
                qDebug("socket LIST receive error");
                ui->loginInfo->append("socket LIST receive error");
        }
        ui->loginInfo->append(Respond);



//        ui->loginInfo->append("accept infomation success");


//            bzero(Respond,MAXSIZE);
//            if(recv(SocketData,Respond,MAXSIZE,0)<0){
//                qDebug("receive list error");
//                exit(0);
//            }
//            ui->direInfo->append(Respond);


        /***************RETR****************/
        memcpy(Sendbuf,"RETR ",strlen("RETR "));
        memcpy(Sendbuf+strlen("RETR "),ui->filename->text().toStdString().data(),strlen(ui->filename->text().toStdString().data()));
        memcpy(Sendbuf+strlen("RETR ")+strlen(ui->filename->text().toStdString().data()),"\r\n",2);
        if(send(SocketControl,Sendbuf,strlen(Sendbuf),0)<0){
            qDebug("send retr Request error");
            exit(0);
        }
         ui->loginInfo->append(Sendbuf);
        ui->loginInfo->append("send RETR Request success");
        if(recv(SocketControl,Respond,MAXSIZE,0)<0){
                    qDebug("socket LIST receive error");
                    ui->loginInfo->append("socket RETR receive error");
        }
        ui->loginInfo->append(Respond);
        int length = sizeof(clientname);
        if((SocketData = accept(SocketConnect,(struct sockaddr*)&clientname,(socklen_t*)&length))<0){
                qDebug("accept infomation error");
                ui->loginInfo->append("accept infomation error");
         }

        char src[256];
        QFile file("/home/javazhe/QTCode/"+ui->filename->text());

        bzero(src,256);
        if ( file.open(QFile::WriteOnly) ) {
          //  for(int k=0;k<j;k++){

               while(recv(SocketData,src,256,0)>0){
                    QTextStream stream( &file );
                    stream <<QObject::trUtf8(src);
                    bzero(src,256);
                }
         //   }


        }

        file.close();
        QMessageBox::information(NULL,"success","file success",QMessageBox::Yes);

    }else {
        QMessageBox::warning(NULL,"error","请输入文件名",QMessageBox::Yes);
    }

    return true;
}

//bool MainWindow::STORRequest(){
//    bzero(Sendbuf,MAXSIZE);
//    bzero(Respond,MAXSIZE);
//    memcpy(Sendbuf,"STOR ",strlen("STOR "));
//    memcpy(Sendbuf+strlen("STOR "),ui->uploadpath->text().toStdString().data(),strlen(ui->uploadpath->text().toStdString().data()));
//    memcpy(Sendbuf+strlen("STOR ")+strlen(ui->uploadpath->text().toStdString().data()),"\r\n",2);
//    if(send(SocketControl,Sendbuf,strlen(Sendbuf),0)<0){
//        qDebug("send stor Request error");
//        exit(0);
//    }
//    ui->loginInfo->append(Sendbuf);
//    ui->loginInfo->append("send stor Request success");
//    if(recv(SocketControl,Respond,MAXSIZE,0)<0){
//                qDebug("socket stor receive error");
//                ui->loginInfo->append("socket stor receive error");
//    }
//    ui->loginInfo->append(Respond);
//    char src[256];
//    while(send(SocketData,src,strlen(src),0)>0){
//         bzero(src,256);
//     }
//    return true;
//}

void MainWindow::on_connectBtn_clicked()
{
    ControlConnect();
}



void MainWindow::on_listBtn_clicked()
{
     PORTRequest();
    ui->loginInfo->append("---------------------------");
    LISTRequest();

}

void MainWindow::on_login_clicked()
{
    FTPLogin();
    ui->loginInfo->append("---------------------------");
    DataConnect();
    PORTRequest();
    QMessageBox::information(NULL, "login", "login success\r\nmodal:主动模式(PORT)", QMessageBox::Yes);

}

void MainWindow::on_quitBtn_clicked()
{
    QUITRequest();
}


/**
 * @brief MainWindow::on_downloadBtn_clicked
 */
void MainWindow::on_downloadBtn_clicked()
{
   RETRRequest();
}





//获取响应码
int MainWindow::RespondCode(char* respondcode){
    int code = 220;
    char* res = new char[3];
    memset(res,0,3);
    //strncpy(res,respondcode,3);
    memcpy(res,respondcode,3);
    code = atoi(res);
    return code;
}

