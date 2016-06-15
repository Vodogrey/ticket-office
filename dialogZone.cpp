#include "dialogZone.h"

dialogZone::dialogZone(QWidget *parent) :
    QDialog(parent)
{
    GUI();
    del = false;
}

void dialogZone::GUI()
{
    m_submit = new QPushButton("Отправить изменения");
    m_revert = new QPushButton("Отменить изменения");
    m_deleteRow = new QPushButton("Удалить выбранные строки");
    m_addRow = new QPushButton("Добавить строку");

    QVBoxLayout* m_layout = new QVBoxLayout();
    setLayout(m_layout);

    model = new zoneSqlTableModel();
    // proxy = new QSortFilterProxyModel();
    //proxy->setSourceModel(model);
    //model = new QSqlRelationalTableModel();

    model->setTable("ZONE");
    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Шоу"));
    model->setHeaderData(2, Qt::Horizontal, tr("Ряд"));
    model->setHeaderData(3, Qt::Horizontal, tr("Начальное место"));
    model->setHeaderData(4, Qt::Horizontal, tr("Конечное место"));
    model->setHeaderData(5, Qt::Horizontal, tr("Цена"));

    model->setJoinMode(QSqlRelationalTableModel::LeftJoin); // чтобы строки с NULL не пропадали
    model->setRelation(1, QSqlRelation("SHOW", "ID", "SHOWNAME"));
    model->setSort(0, Qt::AscendingOrder);
    model->select();

    //model->removeColumn(0); // НЕ режем ID из-за него все к хренам идет, тупо скрываем столб ниже

    view = new QTableView;
    view->setModel(model);
    view->setItemDelegateForColumn(1,new QSqlRelationalDelegate(view));
    view->hideColumn(0);  // здесь скрываем
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

void dialogZone::editZone()
{
    model->relationModel(1)->select();
    //// все фигня, давай по-новой
    //  model->insertColumn(6);
    //   model->setHeaderData(6, Qt::Horizontal, tr("Вычслимый"));
    //   model->setRelation(6, QSqlRelation("SHOW", "ID", "SHOWNAME"));
    //   view->setItemDelegateForColumn(6,new QSqlRelationalDelegate(view));
    //   model->setData(model->index(0,6),33);
}

void dialogZone::clickedSubmit()
{
    if(!isNull() || del) {
        model->submitAll();
        model->select();
        del = false;
    }
}

void dialogZone::clickedRevert()
{
    model->revertAll();

}
void dialogZone::clickedDeleteRow()
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

void dialogZone::clickedAddRow()
{
    if(!isNull() || del) {
        int lastRow = model->rowCount();;
        model->insertRow(lastRow);
        view->selectRow(lastRow);
        view->setFocus();
    }
}


bool dialogZone::isNull()
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

zoneSqlTableModel::zoneSqlTableModel(QObject* parent)
    : QSqlRelationalTableModel(parent)
{

}

/// тупой выход. по возможности подумать, как из index вытянуть model->tableName();
QVariant zoneSqlTableModel::data(const QModelIndex &index, int role) const
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

bool dialogZone::isCanDelete(QString id)
{
    QSqlQuery q;
    q.exec(QString("select IDBLDG from SHOW where IDBLDG = %1").arg(id));
    q.next();
    if(!q.isNull(0)) {
        return false;
    }
    return true;
}
