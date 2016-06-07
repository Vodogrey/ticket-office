#include "mainwindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    isAdmin = true;
    connecting = new connectDtabase();
    dialHall = new dialogHallType();
    dialBldg = new dialogBldg();
    dialShow = new dialogShow();
    dialTimes = new dialogTimes();
    dialMap = new dialogMap();
    dialZone = new dialogZone();
    GUI();
    buttons();
    validateConnect();
}

void MainWindow::GUI()
{
    addMenu();
}

void MainWindow::buttons()
{
    connect(this, SIGNAL(adminEditType()), dialHall, SLOT(editHallType()));
    connect(this, SIGNAL(isConnect()), connecting, SLOT(isConnected()));
    connect(this, SIGNAL(adminEditBldg()), dialBldg, SLOT(editBldg()));
    connect(this, SIGNAL(adminEditShow()), dialShow, SLOT(editShow()));
    connect(this, SIGNAL(adminEditTimes()),dialTimes, SLOT(editTimes()));
    connect(this, SIGNAL(adminEditMap()), dialMap, SLOT(editMap()));
    connect(this, SIGNAL(adminEditZone()), dialZone, SLOT(editZone()));
}

void MainWindow::addMenu()
{
    //delete m_menu;
    QMenu* m_menu = new QMenu("Меню");
    menuBar()->clear();
    menuBar()->addMenu(m_menu);

    qDebug() << "admin" << isAdmin;

    if(isAdmin) {
        m_menu->addAction("Логин под юзером", this, SLOT(setAdmin()));
        m_menu->addAction("Админские штучки", this, SLOT(adminActions()));
    }
    else {
        m_menu->addAction("Логин под админом", this, SLOT(setAdmin()));
    }

    m_menu->addAction("Выход", this, SLOT(close()));


}

void MainWindow::validateConnect()
{
    bool connect = isConnect();
    if(!connect)
        this->close(),
        qDebug() << "close";
}


void MainWindow::setAdmin()
{
    if(isAdmin) {
        isAdmin = false;
        QMessageBox::information(0, "Access", "Now you are user");
    }
    else {
        isAdmin = true;
        QMessageBox::information(0, "Access", "Now you are admin");
    }
    addMenu();
}


void MainWindow::adminActions()
{
    QMessageBox admin;

    admin.setText("Кукусики. что делаем?");

    QPushButton* editBldg = admin.addButton("Управление зданием",QMessageBox::ActionRole);
    QPushButton* editShow = admin.addButton("Управление шоу", QMessageBox::ActionRole);
    QPushButton* editType = admin.addButton("Управление типом здания", QMessageBox::ActionRole);
    QPushButton* editTimes = admin.addButton("Управление Временем", QMessageBox::ActionRole);
    QPushButton* editMap = admin.addButton("Управление картой здания", QMessageBox::ActionRole);
    QPushButton* editZone = admin.addButton("Управление зонами", QMessageBox::ActionRole);

    admin.exec();

        if(admin.clickedButton() == editBldg) {
            emit adminEditBldg();
        }
        if(admin.clickedButton() == editShow) {
            emit adminEditShow();
        }

        if(admin.clickedButton() == editType) {
            emit adminEditType();
        }
        if(admin.clickedButton() == editTimes) {
            emit adminEditTimes();
        }
        if(admin.clickedButton() == editMap) {
            emit adminEditMap();
        }
        if(admin.clickedButton() == editZone) {
            emit adminEditZone();
        }
}
