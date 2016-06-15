#ifndef DIALOGZONE_H
#define DIALOGZONE_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QString>

#include <QDebug>

#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTableView>
#include <QSqlError>
#include <QItemDelegate>
#include <QSqlRelationalDelegate>
#include <QSqlRelationalTableModel>


#include <QObject>
#include <QMessageBox>

#include <QSqlQueryModel>

class zoneSqlTableModel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    zoneSqlTableModel(QObject* parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

class dialogZone : public QDialog
{
    Q_OBJECT
public:
    dialogZone(QWidget *parent = 0);

public slots:
    void editZone();
    void clickedSubmit();
    void clickedRevert();
    void clickedDeleteRow();
    void clickedAddRow();

protected:
    bool isNull();
    void GUI();
    bool isCanDelete(QString id);

signals:

private:
    QPushButton* m_submit;
    QPushButton* m_revert;
    QPushButton* m_deleteRow;
    QPushButton* m_addRow;
    QTableView* view;

    zoneSqlTableModel *model;
//    QSortFilterProxyModel *proxy;
    //QSqlRelationalTableModel* model;

    bool del;
};
#endif // DIALOGZONE_H
