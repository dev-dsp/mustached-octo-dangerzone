#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlError>
#include <QDialog>
#include <QDebug>
#include <QMenu>
#include "DialogTypes.h"
#include "DB.h"

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(QWidget *parent = 0);
    explicit EditDialog(QWidget *parent, QString title, QSqlTableModel * db);
    ~EditDialog();

private slots:
    void on_accept_clicked();
    void on_decline_clicked();
    void on_deleteItem_clicked();

private:
    Ui::EditDialog *ui;
    QSqlTableModel * _db;
};

#endif // EDITDIALOG_H
