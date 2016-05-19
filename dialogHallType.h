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


class MySqlTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    MySqlTableModel(QObject* parent = 0);
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

    MySqlTableModel *model;

    int countAdd;
};


#endif // DIALOGS_H
