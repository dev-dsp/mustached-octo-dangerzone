#include "adddialog.h"
#include "ui_adddialog.h"

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
}

AddDialog::~AddDialog()
{
    delete ui;
}

AddDialog::AddDialog(QWidget *parent, DialogType type, DB * db) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
    this->_type = type;
    this->_db = db;
    switch (type) {
    case District:
        this->setWindowTitle("Добавление района");
        break;
    case DiabetesType:
        this->setWindowTitle("Добавление типа диабета");
        break;
    case DisabilityType:
        this->setWindowTitle("Добавление типа инвалидности");
        break;
    default:
        break;
    }
    this->_status = false;
}

void AddDialog::on_saveButton_clicked()
{
    QSqlQuery q;
    QString query = "INSERT INTO ";
    switch (this->_type) {
    case District:
        query += QString("`tbl_districts`(`name`) VALUES('")+ui->addDialogInput->text()+QString("');");
        break;
    case DiabetesType:
        query += QString("`tbl_diabetes_types`(`type`) VALUES('")+ui->addDialogInput->text()+QString("');");
        break;
    case DisabilityType:
        query += QString("`tbl_disability_types`(`type`) VALUES('")+ui->addDialogInput->text()+QString("');");
        break;
    default:
        break;
    }
    if(!q.exec(query)) {
        QMessageBox::warning(this, "Ошибка", QString("Произошла ошибка во время сохранения.\nТекст ошибки: \"")+
                                             q.lastError().driverText()+
                                             QString("\"\nТекст комманды:\"")+
                                             query+
                                             QString("\""), QMessageBox::Close);
    }
    else {
        QMessageBox::information(this, "Выполнено", "Добавление прошло успешно!");
        ui->addDialogInput->clear();
        qDebug() << q.lastError();
        this->_status = true;
    }
}

void AddDialog::on_closeButton_clicked()
{
    if(this->_status) {
        this->accept();
    }
    else {
        this->reject();
    }
    this->close();

}
