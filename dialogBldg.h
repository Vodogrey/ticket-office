#ifndef DIALOGBLDG_H
#define DIALOGBLDG_H

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


class bdlgSqlTableModel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    bdlgSqlTableModel(QObject* parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

class dialogBldg : public QDialog
{
    Q_OBJECT
public:
    dialogBldg(QWidget *parent = 0);

public slots:
    void editBldg();
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

    bdlgSqlTableModel *model;
    //QSqlRelationalTableModel* model;

    bool del;
};

#endif // DIALOGBLDG_H
