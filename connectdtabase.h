#ifndef CONNECTDTABASE_H
#define CONNECTDTABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QtSql>
#include <QSqlDriverPlugin>
#include <QMessageBox>

class connectDtabase : public QObject
{
    Q_OBJECT
public:
    explicit connectDtabase(QObject *parent = 0);

signals:

public slots:
    bool isConnected();

private:
    bool connected;
};

#endif // CONNECTDTABASE_H
