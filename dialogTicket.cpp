#include "dialogTicket.h"

dialogTicket::dialogTicket(QWidget* parent) :
    QDialog(parent)
{
    GUI();
}
#include <QVBoxLayout>
void dialogTicket::GUI()
{
    m_layout = new QGridLayout();
    m_widget = new QWidget();

    m_cmb_shows = new QComboBox();

    m_layout->addWidget(m_cmb_shows, 0,0,1,1);
    loadShows();

    m_widget->setLayout(m_layout);

    this->setLayout(m_layout);

}

void dialogTicket::loadShows()
{
    QSqlQuery q;
    q.exec(QString("select sh.SHOWNAME, sh.ID "
                   "from TIMES tm inner join SHOW sh on tm.ID_SHOW = sh.ID "
                   "where tm.TIME > SYSDATE"));
    qDebug() << q.lastQuery() <<  q.lastError();
    while(q.next()) {
        QString showName = q.value(0).toString();
        QString id = q.value(1).toString();
        m_cmb_shows->addItem( showName, id );
        qDebug() << showName << id;
        // int index = m_cmb_shows->findData(q.value(1).toInt());
         //m_cmb_shows->setCurrentIndex();
        //qDebug() << "index" << index;
    }
    qDebug() << "g" << m_cmb_shows->itemData(0) << m_cmb_shows->itemText(0);
    qDebug() << "g" << m_cmb_shows->itemData(1) << m_cmb_shows->itemText(1);
}
