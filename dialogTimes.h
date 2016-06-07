#ifndef DIALOGTIMES_H
#define DIALOGTIMES_H

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
#include <QItemDelegate>


#include <QObject>


class timesSqlTableModel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    timesSqlTableModel(QObject* parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

class dialogTimes : public QDialog
{
    Q_OBJECT
public:
    dialogTimes(QWidget *parent = 0);

public slots:
    void editTimes();
    void clickedSubmit();
    void clickedRevert();
    void clickedDeleteRow();
    void clickedAddRow();

    bool isCanDelete(QString id);

protected:
    bool isNull();
    void GUI();

signals:

private:
    QPushButton* m_submit;
    QPushButton* m_revert;
    QPushButton* m_deleteRow;
    QPushButton* m_addRow;
    QTableView* view;

    timesSqlTableModel *model;
};

#endif // DIALOGBLDG_H
