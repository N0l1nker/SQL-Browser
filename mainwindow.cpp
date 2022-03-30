#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QtDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Коннект сигнала объекта подключения к слоту объекта окна в случае успешного подключения,
    //Так же используется в случае обновления таблицы и срабатывания NOTIFY
    connect(&cnctnObj, &SqlConnectionOnThread::connectedToDB, this, &MainWindow::on_db_connection);

    //Коннект сигнала объекта окна к слоту объекта подлючения в случае нажатия пользователем Stop
    connect(this, &MainWindow::cnctStoped, &cnctnObj, &SqlConnectionOnThread::cnctClosed);

    //Коннект сигнала объекта окна к слоту объекта подлючения в случае нажатия кнопки Start
    connect(this, &MainWindow::signal, &cnctnObj, &SqlConnectionOnThread::cnctToDB);

    //Коннект сигнала объекта подключения к слоту объекта окна в случае невозможности подключения
    connect(&cnctnObj, &SqlConnectionOnThread::cntConnect, this, &MainWindow::cntConnect);

    //Перемещение объекта класса подключения к SQL в отдельный поток
    cnctnObj.moveToThread(&newThread);

}

MainWindow::~MainWindow()
{
    delete ui;

    //На случай если перед закрытием окна пользователь не остановил подключение
    newThread.terminate();
}


//Передача данных формы в объект подключения по нажатию на кнопку Start, старт потока
void MainWindow::on_startBtn_clicked()
{
    newThread.start();
    QString hostName = ui->lnEdHostName->text();
    QString dbName= ui->lnEdDatabaseName->text();
    QString userName= ui->lnEdUserName->text();
    QString psw= ui->lnEdPassword->text();
    QString tblName= ui->lnEdTableName->text();
    emit signal(hostName, dbName, userName, psw, tblName);
}


//Прекращение подключения, изменения статусов кнопока, закрытие потока
void MainWindow::on_stopBtn_clicked()
{

    newThread.terminate();
    emit cnctStoped();
    this->ui->stopBtn->setEnabled(false);
    this->ui->startBtn->setEnabled(true);
    ui->tableView->setModel(nullptr);
    ui-> statusbar->showMessage("");
}


//При успешном подключении к БД из объекта подключения передается указатель на модель, в tableview подгружаются данные
void MainWindow::on_db_connection(QSqlTableModel *db_model)
{
    ui->startBtn->setEnabled(false);
    ui->stopBtn->setEnabled(true);
    ui->tableView->setEnabled(true);
    ui-> statusbar->showMessage("Подключено к базе");
    ui->tableView->setModel(db_model);
}


//Кнопка Start становится активна только после ввода пароля
void MainWindow::on_lnEdPassword_editingFinished()
{
    ui->startBtn->setEnabled(true);
}

//Слот с окном предупреждения на случай неправильно введенных данных
void MainWindow::cntConnect()
{
    newThread.terminate();
    QMessageBox messageBox;
    messageBox.information(0,"Ошибка","Введены неправильные данные для подключения");
    messageBox.setFixedSize(500,200);
}



