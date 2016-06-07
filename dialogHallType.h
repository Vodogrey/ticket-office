#ifndef DIALOGS_H
#define DIALOGS_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QString>
#include <QDialogButtonBox>

#include <QDebug>

#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTableView>
#include <QSqlError>


#include <QObject>

#include <QMessageBox>


class hallSqlTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    hallSqlTableModel(QObject* parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};




class dialogHallType : public QDialog
{
    Q_OBJECT
public:
    dialogHallType(QWidget *parent = 0);
protected:
    void GUI();
    bool isNull();
    bool isCanDelete(QString id);

public slots:
    void editHallType();
    void clickedSubmit();
    void clickedRevert();
    void clickedDeleteRow();
    void clickedAddRow();

signals:
    //void setHallType(QString);

private:
    QVBoxLayout *m_layout;
    QPushButton* m_submit;
    QPushButton* m_revert;
    QPushButton* m_deleteRow;
    QPushButton* m_addRow;
    QTableView* view;

    hallSqlTableModel *model;

    int countAdd;
};


#endif // DIALOGS_H
