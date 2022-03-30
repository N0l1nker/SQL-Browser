#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QMainWindow>
#include <QThread>
#include "sqlconnectiononthread.h"
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startBtn_clicked();
    void on_stopBtn_clicked();
    void on_db_connection(QSqlTableModel *db_model);
    void on_lnEdPassword_editingFinished();
    void cntConnect();

signals:
    void cntlineEdited(QString psw);
    void cnctStoped();
    void signal(QString hostName,
                QString dbName,
                QString userName,
                QString psw,
                QString tblName);

private:
    Ui::MainWindow *ui;
    QThread newThread;
    SqlConnectionOnThread cnctnObj;
    QSqlTableModel *dbModelUI;

};
#endif // MAINWINDOW_H
