#include "dialogMap.h"

dialogMap::dialogMap(QWidget *parent) :
    QDialog(parent)
{
    GUI();
    del = false;
}

void dialogMap::GUI()
{
    m_submit = new QPushButton("Отправить изменения");
    m_revert = new QPushButton("Отменить изменения");
    m_deleteRow = new QPushButton("Удалить выбранные строки");
    m_addRow = new QPushButton("Добавить строку");

    QVBoxLayout* m_layout = new QVBoxLayout();
    setLayout(m_layout);


    //buttonBox->addButton(m_revert, QDialogButtonBox::c);

    model = new mapSqlTableModel();
    //model = new QSqlRelationalTableModel();

    model->setTable("MAP");
    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    model->setHeaderData(0, Qt::Horizontal, tr("Здание"));
    model->setHeaderData(1, Qt::Horizontal, tr("Ряд"));
    model->setHeaderData(2, Qt::Horizontal, tr("Число мест"));

    model->setJoinMode(QSqlRelationalTableModel::LeftJoin); // чтобы строки с NULL не пропадали
    model->setRelation(0, QSqlRelation("BLDG", "ID", "NAME"));
    model->setSort(0, Qt::AscendingOrder);
    model->select();

    //model->removeColumn(0); // НЕ режем ID из-за него все к хренам идет, тупо скрываем столб ниже

    view = new QTableView;
    view->setModel(model);
    view->setItemDelegateForColumn(0,new QSqlRelationalDelegate(view));
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

void dialogMap::editMap()
{
  //  model->select();
    model->relationModel(0)->select();
  //  this->exec();
}

void dialogMap::clickedSubmit()
{
    if(!isNull() || del) {
        model->submitAll();
        // model->select();
        del = false;
    }
}

void dialogMap::clickedRevert()
{
    model->revertAll();

}
// косячит удаление здания nope
void dialogMap::clickedDeleteRow()
{
    //    model->removeRow(view->currentIndex().row());
    //    del = true;

    int count = view->selectionModel()->selectedIndexes().count();
    for(int i = 0; i < count; i++) {
        QString id = model->data(model->index(view->selectionModel()->selectedIndexes().at(i).row(), 0, QModelIndex())).toString();
        qDebug() << "id" << id;
        if(canDelete(id)) {
            model->removeRow(view->selectionModel()->selectedIndexes().at(i).row());
            del = true;
        }
        else {
            // эта часть лучше реализована в show && bldg
            QMessageBox::critical(0, QObject::tr("Ошибка удаления"),
                                  /* db.lastError().text()*/ "Есть концерт в этом здании");
        }
    }
}

void dialogMap::clickedAddRow()
{
    if(!isNull() || del) {
        int lastRow = model->rowCount();
        model->insertRow(lastRow);
        view->selectRow(lastRow);
        view->setFocus();
    }
}


bool dialogMap::isNull()
{
    //    qDebug() << view->model()->data(view->currentIndex()).toString();
    //    qDebug() << view->model()->data(view->model()->index(0, 1)).toString();

    for(int i = 0; i < model->rowCount(); i++) {
        if(view->model()->data(view->model()->index(i, 0)).toString().isEmpty()
                || view->model()->data(view->model()->index(i, 1)).toString().isNull()
                || view->model()->data(view->model()->index(i, 2)).toString().isEmpty()
                ) {
            qDebug() << "nope";
            return true;
        }
    }
    return false;
}

mapSqlTableModel::mapSqlTableModel(QObject* parent)
    : QSqlRelationalTableModel(parent)
{

}

/// тупой выход. по возможности подумать, как из index вытянуть model->tableName();
QVariant mapSqlTableModel::data(const QModelIndex &index, int role) const
{
    //QVariant value = QSqlQueryModel::data(index, role);

    QVariant value = QSqlRelationalTableModel::data(index,role);

    switch (role) {
    case Qt::BackgroundColorRole: {  // Цвет фона
        if(QSqlRelationalTableModel::data(QSqlRelationalTableModel::index(index.row(), 0, QModelIndex())).toString().isEmpty()
                || QSqlRelationalTableModel::data(QSqlRelationalTableModel::index(index.row(), 1, QModelIndex())).toString().isEmpty()
                || QSqlRelationalTableModel::data(QSqlRelationalTableModel::index(index.row(), 2, QModelIndex())).toString().isEmpty())
        {
            return qVariantFromValue(QColor(255,0,0));
        }
        else
            return value;
    }
    }
    return value;
}

bool dialogMap::isCanDelete(QString id)
{
    //    QSqlQuery q;
    //    q.exec(QString("select IDTYPE from BLDG where IDTYPE = %1").arg(id));
    //    q.next();
    //    if(!q.isNull(0)) {
    //    return false;
    //    }
    return true;
}
