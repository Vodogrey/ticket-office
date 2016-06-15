#include "dialogHallType.h"


dialogHallType::dialogHallType(QWidget *parent) :
    QDialog(parent)
{
    countAdd = 0;
    GUI();
}

void dialogHallType::GUI()
{
    m_submit = new QPushButton("Отправить изменения");
    m_revert = new QPushButton("Отменить изменения");
    m_deleteRow = new QPushButton("Удалить выбранные строки");
    m_addRow = new QPushButton("Добавить строку");
    m_layout = new QVBoxLayout();
    setLayout(m_layout);



    //buttonBox->addButton(m_revert, QDialogButtonBox::c);

    model = new hallSqlTableModel;
    model->setTable("HALLTYPE");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Тип"));
    model->setSort(0, Qt::AscendingOrder);
    model->select();
    //model->removeColumn(0); // НЕ режем ID из-за него все к хренам идет, тупо скрываем столб ниже

    view = new QTableView;
    view->clearFocus();
    view->setAlternatingRowColors(true);
    view->setModel(model);
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

    resize(800,600);
}

//void dialogHallTypeAdd::addHallType()
//{

//    m_line = new QLineEdit();
//    m_but = new QPushButton("OK");

//    QVBoxLayout *m_layout = new QVBoxLayout();
//    m_layout->addWidget(m_line);
//    setLayout(m_layout);

//    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
//        buttonBox->addButton(m_but, QDialogButtonBox::AcceptRole);

//    m_layout->addWidget(buttonBox);

//    connect(buttonBox,SIGNAL(accepted()), this, SLOT(clickedHallType()));

//    this->exec();
//}

//void dialogHallTypeAdd::clickedHallType()
//{
//    qDebug() << "add";
//    QSqlQuery q;
//    q.exec("select count(*) from HALLTYPE");
//    q.next();
//    int id = q.value(0).toInt() +  1;
//    q.exec(QString("INSERT INTO HALLTYPE(ID,TYPE) VALUES(%1, '%2')").arg(id).arg(m_line->text()));

//    this->close();
//}

void dialogHallType::editHallType()
{
    model->select();
    this->exec();
}

void dialogHallType::clickedSubmit()
{
    if(!isNull()) {
        model->submitAll();
        model->select();
    }
}

void dialogHallType::clickedRevert()
{
    model->revertAll();
}

void dialogHallType::clickedDeleteRow()
{

    int count = view->selectionModel()->selectedIndexes().count();
    qDebug() << count;
    for(int i = 0; i < count; i++) {
        QString id = model->data(model->index(view->selectionModel()->selectedIndexes().at(i).row(), 0, QModelIndex())).toString();
        qDebug() << "id" << id;
        if(isCanDelete(id)) {
            model->removeRow(view->selectionModel()->selectedIndexes().at(i).row());
        }
        else {
            QSqlQuery q;
            q.exec(QString("select TYPE from HALLTYPE where ID = %1").arg(id));
            q.next();
            QString type = q.value(0).toString();
            QMessageBox::critical(0, QObject::tr("Ошибка удаления"),
                                  /* db.lastError().text()*/ QString("Есть здание с типом %1").arg(type));
        }
    }
}

void dialogHallType::clickedAddRow()
{
    int lastRow = model->rowCount();
    model->insertRow(lastRow);
    // model->setData(model->index(lastRow,0),id);
    view->selectRow(lastRow);
    view->setFocus();
    //qDebug() << "ins" << model->lastError().text();
}

bool dialogHallType::isNull()
{
    //    qDebug() << view->model()->data(view->currentIndex()).toString();
    //    qDebug() << view->model()->data(view->model()->index(0, 1)).toString();

    for(int i = 0; i < model->rowCount(); i++) {
        if(view->model()->data(view->model()->index(i, 1)).toString().isEmpty()) {
            qDebug() << "nope";
            return true;
        }
    }
    return false;
}

hallSqlTableModel::hallSqlTableModel(QObject* parent)
    : QSqlTableModel(parent)
{

}

/// тупой выход. по возможности подумать, как из index вытянуть model->tableName();
QVariant hallSqlTableModel::data(const QModelIndex &index, int role) const
{
    //QVariant value = QSqlQueryModel::data(index, role);
    QVariant value = QSqlTableModel::data(index,role);

    QModelIndex myIndex = QSqlTableModel::index(index.row(),1, QModelIndex()) ;

    QString iColor = QSqlTableModel::data(myIndex).toString();


    switch (role) {
    case Qt::BackgroundColorRole: {  // Цвет фона
        if(iColor.isEmpty()) {
            return qVariantFromValue(QColor(255,0,0));
        }
        else
            return value;
    }
    }
    return value;
}


bool dialogHallType::isCanDelete(QString id)
{
    QSqlQuery q;
    q.exec(QString("select IDTYPE from BLDG where IDTYPE = %1").arg(id));
    q.next();
    if(!q.isNull(0)) {
        return false;
    }
    return true;
}
