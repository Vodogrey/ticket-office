#ifndef DIALOGTICKET_H
#define DIALOGTICKET_H

#include <QObject>
#include <QWidget>
#include <QDialog>

#include <QStringList>
#include <QString>

#include <QGridLayout>

#include <QComboBox>


#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include <QDebug>

class dialogTicket : public QDialog
{
    Q_OBJECT
public:
    dialogTicket(QWidget* parent = 0);

protected:
    void GUI();
    void loadShows();

private:
    QGridLayout* m_layout;
    QWidget* m_widget;

    QComboBox* m_cmb_shows;


};

#endif // DIALOGTICKET_H
