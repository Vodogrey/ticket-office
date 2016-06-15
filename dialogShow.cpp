#include "dialogShow.h"

dialogShow::dialogShow(QWidget *parent) :
    QDialog(parent)
{
    GUI();
}

void dialogShow::GUI()
{
    m_submit = new QPushButton("Отправить изменения");
    m_revert = new QPushButton("Отменить изменения");
    m_deleteRow = new QPushButton("Удалить выбранную строку");
    m_addRow = new QPushButton("Добавить строку");

    QVBoxLayout *m_layout = new QVBoxLayout();
    setLayout(m_layout);


    //buttonBox->addButton(m_revert, QDialogButtonBox::c);

    model = new showSqlTableModel();
    model->setTable("SHOW");
    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Здание"));
    model->setHeaderData(2, Qt::Horizontal, tr("Название"));
    model->setHeaderData(3, Qt::Horizontal, tr("Автор"));
    model->setHeaderData(4, Qt::Horizontal, tr("Продолжительность"));

    model->setJoinMode(QSqlRelationalTableModel::LeftJoin); // чтобы строки с NULL не пропадали
    model->setRelation(1, QSqlRelation("BLDG", "ID", "NAME"));
    model->setSort(0, Qt::AscendingOrder);

    model->select();

    //model->removeColumn(0); // НЕ режем ID из-за него все к хренам идет, тупо скрываем столб ниже

    view = new QTableView;
    view->setModel(model);
    view->setItemDelegateForColumn(1,new QSqlRelationalDelegate(view));
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


void dialogShow::editShow()
{
  //  model->select();
    model->relationModel(1)->select();
  //  this->exec();
}

void dialogShow::clickedSubmit()
{
    if(!isNull() || del) {
        model->submitAll();
        model->select();
        del = false;
    }
}

void dialogShow::clickedRevert()
{
    model->revertAll();
    model->select();

}

void dialogShow::clickedDeleteRow()
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
            q.exec(QString("select SHOWNAME from show where ID =  %1").arg(id));
            q.next();
            QString type = q.value(0).toString();
            QMessageBox::critical(0, QObject::tr("Ошибка удаления"),
                     /* db.lastError().text()*/ QString("Есть билеты на шоу %1").arg(type));
        }
    }
}

void dialogShow::clickedAddRow()
{
    int lastRow = model->rowCount();
    model->insertRow(lastRow);
    view->selectRow(lastRow);
    view->setFocus();
}

bool dialogShow::isNull()
{
    //    qDebug() << view->model()->data(view->currentIndex()).toString();
    //    qDebug() << view->model()->data(view->model()->index(0, 1)).toString();

    for(int i = 0; i < model->rowCount(); i++) {
        if(view->model()->data(view->model()->index(i, 1)).toString().isEmpty()
                || view->model()->data(view->model()->index(i, 2)).toString().isEmpty()
                || view->model()->data(view->model()->index(i, 3)).toString().isEmpty()
                || view->model()->data(view->model()->index(i, 4)).toString().isEmpty()) {
            qDebug() << "nope";
            return true;
        }
    }
    return false;
}

bool dialogShow::isCanDelete(QString id)
{
    QSqlQuery q;
    q.exec(QString("select IDBLDG from SHOW where IDBLDG = %1").arg(id));
    q.next();
    if(!q.isNull(0)) {
        return false;
    }
    return true;
}


showSqlTableModel::showSqlTableModel(QObject* parent)
    : QSqlRelationalTableModel(parent)
{

}

QVariant showSqlTableModel::data(const QModelIndex &index, int role) const
{
    //QVariant value = QSqlQueryModel::data(index, role);

    QVariant value = QSqlRelationalTableModel::data(index,role);

    switch (role) {
    case Qt::BackgroundColorRole: {  // Цвет фона
        if(QSqlRelationalTableModel::data(QSqlRelationalTableModel::index(index.row(), 1, QModelIndex())).toString().isEmpty()
                || QSqlRelationalTableModel::data(QSqlRelationalTableModel::index(index.row(), 2, QModelIndex())).toString().isEmpty()
                || QSqlRelationalTableModel::data(QSqlRelationalTableModel::index(index.row(), 3, QModelIndex())).toString().isEmpty()
                || QSqlRelationalTableModel::data(QSqlRelationalTableModel::index(index.row(), 4, QModelIndex())).toString().isEmpty())
        {
            return qVariantFromValue(QColor(255,0,0));
        }
        else
            return value;
    }
    }
    return value;
}
