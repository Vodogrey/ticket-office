#include "dialogBldg.h"

dialogBldg::dialogBldg(QWidget *parent) :
    QDialog(parent)
{

}

void dialogBldg::editBldg()
{
    m_submit = new QPushButton("Отправить изменения");
    m_revert = new QPushButton("Отменить изменения");
    m_deleteRow = new QPushButton("Удалить выбранную строку");
    m_addRow = new QPushButton("Добавить строку");

    QVBoxLayout *m_layout = new QVBoxLayout();
    setLayout(m_layout);


    //buttonBox->addButton(m_revert, QDialogButtonBox::c);

    model = new QSqlRelationalTableModel;
    model->setTable("BLDG");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Название"));
    model->setHeaderData(2, Qt::Horizontal, tr("Тип"));
    model->setHeaderData(3, Qt::Horizontal, tr("Адрес"));

    model->setJoinMode(QSqlRelationalTableModel::LeftJoin); // чтобы строки с NULL не пропадали
    model->setRelation(2, QSqlRelation("HALLTYPE", "ID", "TYPE"));

    model->select();

    //model->removeColumn(0); // НЕ режем ID из-за него все к хренам идет, тупо скрываем столб ниже

    view = new QTableView;
    view->setModel(model);
    view->setItemDelegateForColumn(2,new QSqlRelationalDelegate(view));
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

   this->exec();
   this->clearFocus();
}

void dialogBldg::clickedSubmit()
{
    model->submitAll();
}

void dialogBldg::clickedRevert()
{
    model->revertAll();

}

void dialogBldg::clickedDeleteRow()
{
    model->removeRow(view->currentIndex().row());
}

void dialogBldg::clickedAddRow()
{
    QSqlQuery q;
//    q.exec("select count(*) from HALLTYPE");
//    q.next();
//    int id = q.value(0).toInt() +  1;
//    q.exec(QString("INSERT INTO HALLTYPE(ID,TYPE) VALUES(%1, '')").arg(id));

    q.exec("select MAX(ID) from BLDG");
    q.next();
    int id = q.value(0).toInt() +  1;
    q.lastError();
    qDebug() << "id" << id;
    q.exec(QString("INSERT INTO BLDG(ID) VALUES(%1)").arg(id));


//    model->insertRow(model->rowCount() + 1);
//    model->setData(model->index(model->rowCount()+1,0), model->rowCount() + 1);
//    model->submitAll();
//    model->database().commit();
    model->select();
    qDebug() << model->rowCount();
    qDebug() << model->lastError();
}
