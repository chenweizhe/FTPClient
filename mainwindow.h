#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <QFileDialog>
#define MAXSIZE 1024
#define CONNECTPORT 21
#define p1 20
#define p2 80

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_connectBtn_clicked();

    void on_listBtn_clicked();

    void on_login_clicked();

    void on_quitBtn_clicked();

    void on_downloadBtn_clicked();


private:
    Ui::MainWindow *ui;
    QString m_Address;
    QString m_Username;
    QString m_Password;
    QString m_Logininfo;
    QString m_Direinfo;
    int SocketControl;
    int SocketData;
    int SocketConnect;
   // int RespondCode;
    char Respond[MAXSIZE];
    char Sendbuf[MAXSIZE];
    struct sockaddr_in serverAddr;
    struct sockaddr_in dataAddr;
    //bool RecvRespond();
    bool DataConnect();
    bool ControlConnect();
    bool FTPLogin();
    bool PORTRequest();
    bool LISTRequest();
    bool QUITRequest();
    bool RETRRequest();
    //bool STORRequest();
    int RespondCode(char* respond);
};

#endif // MAINWINDOW_H
