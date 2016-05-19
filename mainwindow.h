#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QString>

#include <QSqlQuery>


#include "connectdtabase.h"
#include "dialogHallType.h"
#include "dialogBldg.h"
#include "dialogShow.h"


#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void GUI();

protected:
    void addMenu();
    void buttons();
    void validateConnect();

signals:
    void adminEditType();
    bool isConnect();
    void adminEditBldg();
    void adminEditShow();

public slots:
    void setAdmin();


protected slots:
    void adminActions();

private:
    connectDtabase* connecting;
    dialogHallType* dialHall;
    dialogBldg* dialBldg;
    dialogShow* dialShow;

    QGridLayout* m_layout;
    QWidget* m_widget;

    QMenu* m_menu;


    bool isAdmin;

};

#endif // MAINWINDOW_H
