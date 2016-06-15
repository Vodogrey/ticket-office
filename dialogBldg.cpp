#include "dialogBldg.h"

dialogBldg::dialogBldg(QWidget *parent) :
    QDialog(parent)
{
    GUI();
    del = false;
}

void dialogBldg::GUI()
{
    m_submit = new QPushButton("Отправить изменения");
    m_revert = new QPushButton("Отменить изменения");
    m_deleteRow = new QPushButton("Удалить выбранные строки");
    m_addRow = new QPushButton("Добавить строку");

    QVBoxLayout *m_layout = new QVBoxLayout();
    setLayout(m_layout);


    //buttonBox->addButton(m_revert, QDialogButtonBox::c);

    model = new bdlgSqlTableModel();
    //model = new QSqlRelationalTableModel();

    model->setTable("BLDG");
    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Название"));
    model->setHeaderData(2, Qt::Horizontal, tr("Тип"));
    model->setHeaderData(3, Qt::Horizontal, tr("Адрес"));

    model->setJoinMode(QSqlRelationalTableModel::LeftJoin); // чтобы строки с NULL не пропадали
    model->setRelation(2, QSqlRelation("HALLTYPE", "ID", "TYPE"));
    model->setSort(0, Qt::AscendingOrder);
    model->select();

    //model->removeColumn(0); // НЕ режем ID из-за него все к хренам идет, тупо скрываем столб ниже

    view = new QTableView;
    view->setModel(model);
    view->setItemDelegateForColumn(2,new QSqlRelationalDelegate(view));
    view->hideColumn(0);  /// здесь
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

void dialogBldg::editBldg()
{
  // model->select();
   model->relationModel(2)->select();
  // this->exec();
}

void dialogBldg::clickedSubmit()
{
    if(!isNull() || del) {
    model->submitAll();
    model->select();
    del = false;
    }
}

void dialogBldg::clickedRevert()
{
    model->revertAll();

}
void dialogBldg::clickedDeleteRow()
{

    int count = view->selectionModel()->selectedIndexes().count();
    for(int i = 0; i < count; i++) {
        QString id = model->data(model->index(view->selectionModel()->selectedIndexes().at(i).row(), 0, QModelIndex())).toString();
        qDebug() << "id" << id;
        if(isCanDelete(id)) {
            model->removeRow(view->selectionModel()->selectedIndexes().at(i).row());
            del = true;
        }
        else {
            QSqlQuery q;
            q.exec(QString("select NAME from BLDG where ID = %1").arg(id));
            q.next();
            QString type = q.value(0).toString();
            QMessageBox::critical(0, QObject::tr("Ошибка удаления"),
                     /* db.lastError().text()*/ QString("Есть концерт в здании %1").arg(type));
        }
    }
}

void dialogBldg::clickedAddRow()
{
    if(!isNull() || del) {
    int lastRow = model->rowCount();;
    model->insertRow(lastRow);
    view->selectRow(lastRow);
    view->setFocus();
    }
}


bool dialogBldg::isNull()
{
//    qDebug() << view->model()->data(view->currentIndex()).toString();
//    qDebug() << view->model()->data(view->model()->index(0, 1)).toString();

    for(int i = 0; i < model->rowCount(); i++) {
       if(view->model()->data(view->model()->index(i, 1)).toString().isEmpty()
               || view->model()->data(view->model()->index(i, 2)).toString().isNull()
               || view->model()->data(view->model()->index(i, 3)).toString().isEmpty()
               ) {
           qDebug() << "nope";
           return true;
       }
    }
    return false;
}

bdlgSqlTableModel::bdlgSqlTableModel(QObject* parent)
    : QSqlRelationalTableModel(parent)
{

}

/// тупой выход. по возможности подумать, как из index вытянуть model->tableName();
QVariant bdlgSqlTableModel::data(const QModelIndex &index, int role) const
{
    //QVariant value = QSqlQueryModel::data(index, role);

    QVariant value = QSqlRelationalTableModel::data(index,role);

    QString iColor = QSqlRelationalTableModel::data(QSqlRelationalTableModel::index(index.row(), 1, QModelIndex())).toString();
    QString iColor2 = QSqlRelationalTableModel::data(QSqlRelationalTableModel::index(index.row(), 3, QModelIndex())).toString();

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

bool dialogBldg::isCanDelete(QString id)
{
    QSqlQuery q;
    q.exec(QString("select IDBLDG from SHOW where IDBLDG = %1").arg(id));
    q.next();
    if(!q.isNull(0)) {
    return false;
    }
    return true;
}
