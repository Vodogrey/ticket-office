#include "dialogTimes.h"

dialogTimes::dialogTimes(QWidget *parent) :
    QDialog(parent)
{
    GUI();
}

void dialogTimes::GUI()
{
    m_submit = new QPushButton("Отправить изменения");
    m_revert = new QPushButton("Отменить изменения");
    m_deleteRow = new QPushButton("Удалить выбранную строку");
    m_addRow = new QPushButton("Добавить строку");

    QVBoxLayout *m_layout = new QVBoxLayout();
    setLayout(m_layout);


    //buttonBox->addButton(m_revert, QDialogButtonBox::c);

    model = new timesSqlTableModel();
    //model = new QSqlRelationalTableModel();

    model->setTable("TIMES");
    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    model->setHeaderData(0, Qt::Horizontal, tr("Шоу"));
    model->setHeaderData(1, Qt::Horizontal, tr("Время"));

    model->setJoinMode(QSqlRelationalTableModel::LeftJoin); // чтобы строки с NULL не пропадали
    model->setRelation(0, QSqlRelation("BLDG", "ID", "NAME"));
    model->setSort(0, Qt::AscendingOrder);
    model->select();

    //model->removeColumn(0); // НЕ режем ID из-за него все к хренам идет, тупо скрываем столб ниже

    view = new QTableView;
    view->setModel(model);
    view->setItemDelegateForColumn(0,new QSqlRelationalDelegate(view));
   // view->hideColumn(0);  /// здесь
    view->resizeColumnsToContents();


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

void dialogTimes::editTimes()
{
   model->select();
   this->exec();
   this->clearFocus();
}

void dialogTimes::clickedSubmit()
{
    if(!isNull())
    model->submitAll();
}

void dialogTimes::clickedRevert()
{
    model->revertAll();

}

void dialogTimes::clickedDeleteRow()
{
    model->removeRow(view->currentIndex().row());
}

void dialogTimes::clickedAddRow()
{
    int lastRow = model->rowCount();;
    model->insertRow(lastRow);
    view->selectRow(lastRow);
    view->setFocus();
}


bool dialogTimes::isNull()
{
//    qDebug() << view->model()->data(view->currentIndex()).toString();
//    qDebug() << view->model()->data(view->model()->index(0, 1)).toString();

    for(int i = 0; i < model->rowCount(); i++) {
       if(view->model()->data(view->model()->index(i, 1)).toString().isEmpty()
               || view->model()->data(view->model()->index(i, 2)).toString().isEmpty()) {
           qDebug() << "nope";
           return true;
       }
    }
    return false;
}

timesSqlTableModel::timesSqlTableModel(QObject* parent)
    : QSqlRelationalTableModel(parent)
{

}

/// тупой выход. по возможности подумать, как из index вытянуть model->tableName();
QVariant timesSqlTableModel::data(const QModelIndex &index, int role) const
{
    //QVariant value = QSqlQueryModel::data(index, role);

    QVariant value = QSqlRelationalTableModel::data(index,role);

    QString iColor = QSqlRelationalTableModel::data(QSqlRelationalTableModel::index(index.row(), 0, QModelIndex())).toString();
    QString iColor2 = QSqlRelationalTableModel::data(QSqlRelationalTableModel::index(index.row(), 1, QModelIndex())).toString();

    switch (role) {
    case Qt::BackgroundColorRole: {  // Цвет фона
        if(iColor.isEmpty() || iColor2.isEmpty())
        {
            return qVariantFromValue(QColor(255,0,0));
        }
        else
            return value;
    }
    }
    return value;
}

bool dialogTimes::isCanDelete(QString id)
{
    QSqlQuery q;
    q.exec(QString("select IDTYPE from BLDG where IDTYPE = %1").arg(id));
    q.next();
    if(!q.isNull(0)) {
    return false;
    }
    return true;
}
