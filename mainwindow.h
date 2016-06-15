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

#include <QTabWidget>


#include "connectdtabase.h"
#include "dialogHallType.h"
#include "dialogBldg.h"
#include "dialogShow.h"
#include "dialogTimes.h"
#include "dialogMap.h"
#include "dialogZone.h"
#include "dialogTicket.h"


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
    void adminEditTimes();
    void adminEditMap();
    void adminEditZone();

public slots:
    void setAdmin();
    void updateRelation(int page);


protected slots:
    void adminActions();

private:
    connectDtabase* connecting;
    dialogHallType* dialHall;
    dialogBldg* dialBldg;
    dialogShow* dialShow;
    dialogTimes* dialTimes;
    dialogMap* dialMap;
    dialogZone* dialZone;
    dialogTicket* dialTicket;


    QGridLayout* m_layout;
    QWidget* m_widget;

    QMenu* m_menu;

    QTabWidget *tabWidget;


    bool isAdmin;

};

#endif // MAINWINDOW_H
