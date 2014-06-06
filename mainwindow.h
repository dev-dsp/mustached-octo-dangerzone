#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QTextCodec>
#include "DB.h"
#include "editdialog.h"
#include "adddialog.h"
#include "dateeditdelegate.h"
#include "exportdialog.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_addButton_clicked();

    void on_searchButton_clicked();

    void on_add_district_triggered();

    void on_add_diabetesType_triggered();

    void on_add_disabilityType_triggered();

    void on_edit_district_triggered();

    void on_edit_diabetesType_triggered();

    void on_edit_disabilityType_triggered();

    void on_showAll_clicked();

    void on_exportButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::MainWindow * ui;
    DB * db;
};

#endif // MAINWINDOW_H
