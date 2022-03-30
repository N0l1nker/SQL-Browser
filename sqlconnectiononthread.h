#ifndef SQLCONNECTIONONTHREAD_H
#define SQLCONNECTIONONTHREAD_H
#include <QSqlTableModel>
#include <QObject>
#include <QtSql>
#include <QSqlDriver>
#include <QVariant>
#include <QVector>
class SqlConnectionOnThread : public QObject
{
    Q_OBJECT

public:
    explicit SqlConnectionOnThread(QObject *parent = nullptr);


signals:
    void connectedToDB(QSqlTableModel *dbModel);
    void cntConnect();


public slots:
    void cnctToDB(QString hostName,
                  QString dbName,
                  QString userName,
                  QString psw,
                  QString tblName);
    void cnctClosed();
    void SlotNotify(const QString &iName, QSqlDriver::NotificationSource iSource, const QVariant &iPayload);
private:
    QSqlDatabase db;
    QSqlTableModel *dbModel;
    QSqlTableModel *dbModelUpdated;
};

#endif // SQLCONNECTIONONTHREAD_H
