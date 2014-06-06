#include "editdialog.h"
#include "ui_editdialog.h"

EditDialog::EditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDialog)
{
    ui->setupUi(this);
}

EditDialog::EditDialog(QWidget *parent, QString title, QSqlTableModel * db) :
    QDialog(parent),
    ui(new Ui::EditDialog)
{
    ui->setupUi(this);
    this->_db = db;
    this->setWindowTitle(title);
    if(!db->select()){
        qDebug() << db->query().lastError();
    }
    db->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->editView->setModel(db);
    ui->editView->setModelColumn(1);
}

EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::on_accept_clicked()
{
    if(_db->submitAll()) {
        QMessageBox::information(this, "Выполнено", "Сохранено успешно", QMessageBox::Ok);
        this->close();
    }
    else {
        QMessageBox::warning(this, "Ошибка", QString("Произошла ошибка во время сохранения.\nТекст ошибки: \"")+
                                             _db->lastError().driverText()+
                                             QString("\""), QMessageBox::Close);
    }
}

void EditDialog::on_decline_clicked()
{
    if(QMessageBox::question(this, "Закрытие", "Убедитесь, что сохранили все изменения!\nЗакрыть?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
        this->close();
    }
}

void EditDialog::on_deleteItem_clicked()
{
    QSqlError err;
    for(int i=0; i< ui->editView->selectionModel()->selectedRows().toVector().size(); i++){
        err = DB::deleteRelationRow(this->_db, ui->editView->selectionModel()->selectedRows().toVector()[i].data().toInt());
        if(err.type() != QSqlError::NoError) {
            QMessageBox::warning(this, "Ошибка", QString("Произошла ошибка во время сохранения.\nТекст ошибки: \"")+
                                                 err.driverText()+
                                                 QString("\""), QMessageBox::Close);
            this->_db->select();
            return;
        }
    }
    QMessageBox::information(this, "Выполнено", "Удалено успешно!", QMessageBox::Ok);
    this->_db->select();
}
