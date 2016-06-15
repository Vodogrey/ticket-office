#include "connectdtabase.h"

connectDtabase::connectDtabase(QObject *parent) : QObject(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QOCI");
    db.setHostName("localhost");
    db.setDatabaseName("XE");
    db.setUserName("MYDB");
    db.setPassword("123");
    db.setPort(1521);
    bool ok = db.open();

    if (!ok) {
        QMessageBox::critical(0, QObject::tr("Ошибка соединения"),
                              /* db.lastError().text()*/ "Обратитесь за помощью к Создателю");
    }
    else {
        QMessageBox::information(0, QObject::tr("Соединение установлено"),
                                 "Добро пожаловать");
        connected = true;
    }

    // qDebug() << "drivers" << QSqlDatabase::drivers();
    //qDebug() << "libs" << QApplication::libraryPaths();
    // QPluginLoader load;
    // load.setFileName("/opt/Qt/5.5/gcc_64/plugins/sqldrivers/libqsqloci.so");
    //qDebug() << load.load();
    //   qDebug() << "loads" << load.errorString();
}

bool connectDtabase::isConnected()
{
    return connected;
}
