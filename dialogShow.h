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

#include <QMessageBox>

class showSqlTableModel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    showSqlTableModel(QObject* parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

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

protected:
    void GUI();
    bool isNull();
    bool isCanDelete(QString id);

signals:

private:
    QPushButton* m_submit;
    QPushButton* m_revert;
    QPushButton* m_deleteRow;
    QPushButton* m_addRow;
    QTableView* view;

    showSqlTableModel *model;
    bool del;
};

#endif // DIALOGSHOW_H
