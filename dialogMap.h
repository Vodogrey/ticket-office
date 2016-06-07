#ifndef DIALOGMAP_H
#define DIALOGMAP_H

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

class mapSqlTableModel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    mapSqlTableModel(QObject* parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

class dialogMap : public QDialog
{
    Q_OBJECT
public:
    dialogMap(QWidget* parent = 0);

public slots:
    void editMap();
    void clickedSubmit();
    void clickedRevert();
    void clickedDeleteRow();
    void clickedAddRow();

    bool isCanDelete(QString id);

protected:
    bool isNull();
    void GUI();

signals:
    bool canDelete(QString);

private:
    QPushButton* m_submit;
    QPushButton* m_revert;
    QPushButton* m_deleteRow;
    QPushButton* m_addRow;
    QTableView* view;

    mapSqlTableModel *model;

    bool del;
};

#endif // DIALOGMAP_H
