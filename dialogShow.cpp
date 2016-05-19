#include "dialogShow.h"

dialogShow::dialogShow(QWidget *parent) :
    QDialog(parent)
{

}


void dialogShow::editShow()
{

    m_submit = new QPushButton("Отправить изменения");
    m_revert = new QPushButton("Отменить изменения");
    m_deleteRow = new QPushButton("Удалить выбранную строку");
    m_addRow = new QPushButton("Добавить строку");

    QVBoxLayout *m_layout = new QVBoxLayout();
    setLayout(m_layout);


    //buttonBox->addButton(m_revert, QDialogButtonBox::c);

    model = new QSqlRelationalTableModel;
    model->setTable("SHOW");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Здание"));
    model->setHeaderData(2, Qt::Horizontal, tr("Название"));
    model->setHeaderData(3, Qt::Horizontal, tr("Автор"));
    model->setHeaderData(4, Qt::Horizontal, tr("Продолжительность"));

    model->setJoinMode(QSqlRelationalTableModel::LeftJoin); // чтобы строки с NULL не пропадали
    model->setRelation(1, QSqlRelation("BLDG", "ID", "NAME"));

    model->select();

    //model->removeColumn(0); // НЕ режем ID из-за него все к хренам идет, тупо скрываем столб ниже

    view = new QTableView;
    view->setModel(model);
    view->setItemDelegateForColumn(1,new QSqlRelationalDelegate(view));
    view->hideColumn(0);  /// здесь


    m_layout->addWidget(view);
    m_layout->addWidget(m_submit);
    m_layout->addWidget(m_revert);
    m_layout->addWidget(m_deleteRow);
    m_layout->addWidget(m_addRow);

    connect(m_submit,SIGNAL(released()), this, SLOT(clickedSubmit()));
    connect(m_revert, SIGNAL(released()),this, SLOT(clickedRevert()));
    connect(m_deleteRow, SIGNAL(released()), this, SLOT(clickedDeleteRow()));
    connect(m_addRow, SIGNAL(released()), this, SLOT(clickedAddRow()));

}

void dialogShow::clickedSubmit()
{
    model->submitAll();
    model->select();
}

void dialogShow::clickedRevert()
{
    model->revertAll();
    model->select();

}

void dialogShow::clickedDeleteRow()
{
    model->removeRow(view->currentIndex().row());
}

void dialogShow::clickedAddRow()
{
    QSqlQuery q;
//    q.exec("select count(*) from HALLTYPE");
//    q.next();
//    int id = q.value(0).toInt() +  1;
//    q.exec(QString("INSERT INTO HALLTYPE(ID,TYPE) VALUES(%1, '')").arg(id));

    q.exec("select MAX(ID) from SHOW");
    q.next();
    int id = q.value(0).toInt() +  1;
    q.lastError();
    qDebug() << "id" << id;
    q.exec(QString("INSERT INTO SHOW(ID) VALUES(%1)").arg(id));



//    model->insertRow(model->rowCount() + 1);
//    model->setData(model->index(model->rowCount()+1,0), model->rowCount() + 1);
//    model->submitAll();
//    model->database().commit();
    model->select();
    qDebug() << model->rowCount();
    qDebug() << model->lastError();
}
