#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QFileDialog>

#include "tblmodel.h"

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = 0);
    explicit ExportDialog(QWidget *parent, TblModel *m);
    ~ExportDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ExportDialog *ui;
    void exportCSV();
    TblModel * model;
    QList<int> _getChecked();
};

#endif // EXPORTDIALOG_H
