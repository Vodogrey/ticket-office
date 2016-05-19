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

signals:

private:
    QPushButton* m_submit;
    QPushButton* m_revert;
    QPushButton* m_deleteRow;
    QPushButton* m_addRow;
    QTableView* view;

    QSqlRelationalTableModel *model;
};

#endif // DIALOGBLDG_H
