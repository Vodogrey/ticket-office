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
    m_deleteRow = new QPushButton("Удалить выбранную строку");
    m_addRow = new QPushButton("Добавить строку");
    m_layout = new QVBoxLayout();
    setLayout(m_layout);



    //buttonBox->addButton(m_revert, QDialogButtonBox::c);

    model = new MySqlTableModel;
    model->setTable("HALLTYPE");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Тип"));
    model->select();
    //model->removeColumn(0); // НЕ режем ID из-за него все к хренам идет, тупо скрываем столб ниже

    view = new QTableView;

    view->setAlternatingRowColors(true);


    view->clearFocus();
    view->setAlternatingRowColors(true);
    view->setModel(model);
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
    if(!isNull())
        model->submitAll();
}

void dialogHallType::clickedRevert()
{
    model->revertAll();
    if(countAdd !=0) {
        QSqlQuery q;
        q.exec(QString("alter sequence halltype_seq increment by -%1").arg(countAdd));
        q.exec("select halltype_seq.nextval from halltype");
        q.exec("alter sequence halltype_seq increment by 1");
        countAdd = 0;
    }

}

void dialogHallType::clickedDeleteRow()
{
    model->removeRow(view->currentIndex().row());
}

void dialogHallType::clickedAddRow()
{

// god is here
    QSqlQuery q;
    int lastRow = model->rowCount();
    q.exec("select halltype_seq.nextval from halltype"); // и что тут не нравится?? // обязательно nextval. придется считать записи
    q.next();
    int id = q.value(0).toInt();
    countAdd++;

    qDebug() << "id" << id;
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

MySqlTableModel::MySqlTableModel(QObject* parent)
    : QSqlTableModel(parent)
{

}


QVariant MySqlTableModel::data(const QModelIndex &index, int role) const
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
