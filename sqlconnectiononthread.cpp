#include "sqlconnectiononthread.h"
#include <QtSql>
#include <QtDebug>
#include <QSqlTableModel>
#include <QTableView>
#include <mainwindow.h>
#include <QSqlDriver>
#include <QVariant>
#include <QMessageBox>
SqlConnectionOnThread::SqlConnectionOnThread(QObject *parent)
    : QObject{parent}

{

}

//Удаление подключения
void SqlConnectionOnThread::cnctClosed()
{
    db.removeDatabase("QPSQL");
}

//Основной слот для подключения к БД. В качестве параметров из объекта окна передаюся ввёденные пользователем данные
void SqlConnectionOnThread::cnctToDB(QString hostName,QString dbName, QString userName, QString psw, QString tblName)
{

    //Заполнение свойств объекта базы
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(hostName);
    db.setDatabaseName(dbName);
    db.setUserName(userName);
    db.setPassword(psw);

   if (db.open())
    {
       //Подпись на появление NOTIFY
       db.driver()->subscribeToNotification("Table changed");
       connect(
                   db.driver(),
                   SIGNAL(notification(const QString &, QSqlDriver::NotificationSource, const QVariant &)),
                   this,
                   SLOT(SlotNotify(const QString &, QSqlDriver::NotificationSource, const QVariant &))
                );

        dbModel = new QSqlTableModel(this, db);
        dbModel->setTable(tblName);
        dbModel->select();
        emit connectedToDB(dbModel);
    }
    else
    {
      //Сигнал для появления окна с ошибкой
      emit cntConnect();
    }

}

//При срабатывании нотифая бд объект подключения к БД создает новую модель и передает её в объект MainWindow для перезагрузки таблицы
void SqlConnectionOnThread::SlotNotify(const QString &iName, QSqlDriver::NotificationSource iSource, const QVariant &iPayload)
{
    dbModelUpdated = new QSqlTableModel(this, db);
    dbModelUpdated->setTable(db.tables().value(0));
    dbModelUpdated->select();
    emit connectedToDB(dbModelUpdated);
}


