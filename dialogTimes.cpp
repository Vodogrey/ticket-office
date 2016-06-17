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
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, tr("Шоу"));
    model->setHeaderData(2, Qt::Horizontal, tr("Время"));

    model->setJoinMode(QSqlRelationalTableModel::LeftJoin); // чтобы строки с NULL не пропадали
    model->setRelation(1, QSqlRelation("SHOW", "ID", "SHOWNAME"));
    qDebug() << "name" << model->relationModel(1)->objectName();
    qDebug() << "disp" <<model->relation(1).displayColumn();
    qDebug() << "апшипка" << model->lastError();
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
#include <QVariant>
#include <QSqlRecord>
void dialogTimes::editTimes()
{
    //    model->select();
    //    this->exec();
    //    this->clearFocus();
    model->relationModel(1)->select();
    qDebug() << "and there" << model->data(model->index(0,2));
    qDebug() << "edit times" << model->data(model->index(0, 1));
}

void dialogTimes::clickedSubmit()
{
    if((!isNull() && isCanAdd()) || del) {
        qDebug () << "del";
        model->submitAll();
        // model->select();
        del = false;
        qDebug() << "model error" << model->lastError();
    }
}

void dialogTimes::clickedRevert()
{
    model->revertAll();

}

void dialogTimes::clickedDeleteRow()
{
    model->removeRow(view->currentIndex().row());
    del = true;
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
            qDebug() << "nope" << del;
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

bool dialogTimes::isCanAdd()
{
    bool can = true;
    for(int i = 0; i < model->rowCount(); i++) {
        if(model->isDirty(model->index(i,2)) || model->isDirty(model->index(i,1))) {
            QDateTime date = model->data(model->index(i,2)).toDateTime(); // time from times
            QString id = model->data(model->index(i,0)).toString(); // id from times
            QString showName = model->data(model->index(i,1)).toString();

// в попец все это дело. берем и гарантируем единственное название шоу.
        //    qDebug() << "showName" << showName;
            QSqlQuery getBldgId;
            getBldgId.exec(QString("select IDBLDG, DURATION "
                                   "from SHOW "
                                   "where SHOWNAME LIKE '%1'").arg(showName));
            getBldgId.next();
            QString idBldg = getBldgId.value(0).toString();
            QString dur = getBldgId.value(1).toString();
            qDebug() << "idBldg" << idBldg;
//            model->setRelation(1, QSqlRelation("SHOW", "ID", "SHOWNAME, ID_SHOW as ID_SHOW"));
//           // model->select();
//            model->relationModel(1)->select();
//            QString idShow = model->data(model->index(i,2)).toString();
//            model->setRelation(1, QSqlRelation("SHOW", "ID", "SHOWNAME"));
//            model->relationModel(1)->select();

//            qDebug() <<"idShow" << idShow;

            //QString idBldg = model->relationModel(1)->record(1).value("IDBLDG").toString();
             //я не понамаю, почему это работает, поэтому не трожь
            //все просто. это не работает. нам нужен индекс строки в таблице шоу

            ////////////////////
//            date.replace("T",":");
            qDebug() << "date" << date;


            QSqlQuery q;
//            q.exec(QString("select sw.SHOWNAME, tm.TIME + (sw.DURATION)*(1/24/60) " // тут все нахрен не нужно, но что-то же
//                           // надо селектить, ибо делаю проверку на наличие чего-либо. нужно
//                           // так же добавить проверку, что шоу влезает между двумя другими
//                           "from TIMES tm inner join SHOW sw on tm.ID_SHOW=sw.ID "
//                           "where (tm.TIME + (sw.DURATION)*(1/24/60)) >= to_date('%1', 'yyyy-mm-dd:hh24:mi:ss') "
//                           "and sw.IDBLDG = %3 "
//                           "and not tm.ID = %2").arg(date).arg(id).arg(idBldg));
//            q.exec(QString("select tm.id, sw.SHOWNAME, tm.TIME + (sw.DURATION)*(1/24/60) "
//                           "from TIMES tm inner join SHOW sw on tm.ID_SHOW=sw.ID "
//                           "where ((tm.TIME + (sw.DURATION)*(1/24/60)) >= to_date('%1', 'yyyy-mm-dd:hh24:mi:ss') "
//                           "or tm.TIME - (sw.DURATION)*(1/24/60) > to_date('2016-01-01:00:00:00', 'yyyy-mm-dd:hh24:mi:ss')) "
//                           "and sw.IDBLDG = %3 "
//                           "and not tm.ID = %2").arg(date).arg(id).arg(idBldg));

// ЭТО И ЕСТЬ ГЕИАЛЬНОЕ РЕШЕНИЕ!!!
// ЛУЧШЕЕ ТВОРЕНИЕ ЩИЛАВЕЩИСТВА
            q.exec(QString("select tm.TIME, tm.TIME + sw.DURATION*(1/24/60), sw.IDBLDG, sw.DURATION "
                           "from TIMES tm inner join SHOW sw on tm.ID_SHOW = sw.ID "
                           "where sw.IDBLDG = %1 and not tm.ID = %2 "
                           "order by TIME desc").arg(idBldg).arg(id));
          //  q.next();
            qDebug() << "query" << q.lastQuery();
            QDateTime endPrevious /*= q.value(1).toDateTime()*/;
            QDateTime startPrevious/* = q.value(0).toDateTime()*/;
            while(q.value(1) > date || q.isNull(1)) {
                q.next();
                qDebug() << "startPrev endPrev" << q.value(0).toString() << q.value(1).toString();
                endPrevious = q.value(1).toDateTime();
                startPrevious = q.value(0).toDateTime();
            }
            QDateTime startNext = q.value(0).toDateTime();
            qDebug() << "if next" << startNext;
            if(q.previous()) {
                startNext = q.value(0).toDateTime();
            }
            else {
                qDebug() << "i'm there";
                if(endPrevious < date)
                continue;
            }

            qDebug() << "dates" << endPrevious.secsTo(startNext) << endPrevious.secsTo(date) << (dur.toInt() * 60);
            qDebug() << date.secsTo(startPrevious) << (dur.toInt() * 60);
            qDebug() << "endPrevious" << endPrevious << "\nstartNext" << startNext;
            if(((endPrevious.secsTo(startNext) <= (dur.toInt() * 60)) && date > endPrevious)
                    || (date.secsTo(startPrevious) <= (dur.toInt() * 60))) {
                QString type = model->data(model->index(i,1)).toString();
                QMessageBox::critical(0, QObject::tr("Ошибка добавления"),
                                     /* db.lastError().text()*/ QString("Шоу %1 в строке %2 пересекается с другим").arg(type).arg(i+1));
                return false;
            }

            qDebug() << "query" << q.lastQuery() << q.lastError();
//            if(!q.isNull(0)) {
//                QString type = model->data(model->index(i,1)).toString();
//                qDebug() << "type" << type;
//                QMessageBox::critical(0, QObject::tr("Ошибка добавления"),
//                                      /* db.lastError().text()*/ QString("Шоу %1 пересекается с другим").arg(type));
//                can = false;
//            }
            qDebug() << "datetime" << q.value(1).toDateTime();
//            if(q.value(1).toDateTime() > date) { // вот это проверяет, не попадает ли конец шоу в начало добавляемого
//                q.next();
//                if(q.value().toDateTime() > )
//            }
        }
    }
    qDebug() << "can" << can;
    return can;
    //    if(q.isNull(0))
    //        return true;
    //    qDebug() << "cant add";
    //    return false;

}
//select sw.SHOWNAME, tm.TIME + (sw.DURATION)*(1/24/60), sw.IDBLDG
//from TIMES tm inner join SHOW sw on tm.ID_SHOW=sw.ID
//where (tm.TIME + (sw.DURATION)*(1/24/60)) >= to_date('2000-01-01:00:00:00', 'yyyy-mm-dd:hh24:mi:ss') and
//--нужна проверка на здание. пока хрень
//sw.IDBLDG
//-- and not tm.ID = 0
