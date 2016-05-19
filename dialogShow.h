#ifndef DIALOGSHOW_H
#define DIALOGSHOW_H

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

class dialogShow : public QDialog
{
    Q_OBJECT
public:
    dialogShow(QWidget *parent = 0);

public slots:
    void editShow();
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

#endif // DIALOGSHOW_H
