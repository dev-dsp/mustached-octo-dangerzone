#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "DialogTypes.h"
#include "DB.h"

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = 0);
    explicit AddDialog(QWidget *parent, DialogType type, DB * db);
    ~AddDialog();

private slots:
    void on_saveButton_clicked();
    void on_closeButton_clicked();

private:
    Ui::AddDialog *ui;
    DialogType _type;
    DB * _db;
    bool _status;
};

#endif // ADDDIALOG_H
