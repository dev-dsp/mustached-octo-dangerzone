#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(QWidget *parent, TblModel * m) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
    this->model = m;
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

QList<int> ExportDialog::_getChecked(){
    QList<int> list;
    QObjectList children = ui->groupBox->children();
    for(int i = 1; i < children.size(); i++){
        QCheckBox * tmp = qobject_cast<QCheckBox *>(children.at(i));
        if(tmp->isChecked()){
            list.append(model->fieldIndex(tmp->objectName()));
        }
    }
    return list;
}

void ExportDialog::exportCSV()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Выберите файл для сохранения", QString(), "CSV (*.csv);;");
    QString CSV;
    QList<int> cols = this->_getChecked();
    for(int i = 0; i < model->rowCount(); i++){
        foreach (int col, cols) {
            CSV += QString("\"")+model->data(model->index(i,col)).toString()+QString("\",");
        }
        CSV.remove(CSV.length()-1,1);
        CSV += QString("\n");
    }
    CSV.remove(CSV.length()-1,1);
    QFile file(fileName);
    file.open(QIODevice::Append | QIODevice::Text);
    file.write(CSV.toStdString().c_str());
    file.close();
}

void ExportDialog::on_buttonBox_accepted()
{
    exportCSV();
}
