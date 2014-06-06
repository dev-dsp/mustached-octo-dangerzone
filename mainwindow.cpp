#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
    ui->setupUi(this);
    this->setWindowTitle("Система учёта клиентов");
    ui->dateAdd->setDate(QDate::currentDate());
    ui->birthDate->setDate(QDate::currentDate());
    ui->validity->setDate(QDate::currentDate());
    ui->dateAdd->setDisplayFormat(QString("dd.MM.yyyy"));
    db = new DB(this);
    ui->clientTable->setModel(db->getModel());
    ui->clientTable->resizeColumnsToContents();
    //qDebug() << db->getModel()->relationModel(db->getModel()->fieldIndex("district"))->record().fieldName(1);
    db->getModel()->relationModel(db->getModel()->fieldIndex("district"))->setFilter("`deleted` = 0");
    db->getModel()->relationModel(db->getModel()->fieldIndex("diabetes_type"))->setFilter("`deleted` = 0");
    db->getModel()->relationModel(db->getModel()->fieldIndex("disability_type"))->setFilter("`deleted` = 0");

    ui->district->setModel(db->getModel()->relationModel(db->getModel()->fieldIndex("district")));
    ui->district->setModelColumn(1);

    ui->diabetesType->setModel(db->getModel()->relationModel(db->getModel()->fieldIndex("diabetes_type")));
    ui->diabetesType->setModelColumn(1);

    ui->disabilityType->setModel(db->getModel()->relationModel(db->getModel()->fieldIndex("disability_type")));
    ui->disabilityType->setModelColumn(1);

    ui->districtS->setModel(db->getModel()->relationModel(db->getModel()->fieldIndex("district")));
    ui->districtS->setModelColumn(1);

    ui->disabilityTypeS->setModel(db->getModel()->relationModel(db->getModel()->fieldIndex("disability_type")));
    ui->disabilityTypeS->setModelColumn(1);

    ui->diabetesTypeS->setModel(db->getModel()->relationModel(db->getModel()->fieldIndex("diabetes_type")));
    ui->diabetesTypeS->setModelColumn(1);
    //db->getModel()->select();
    ui->clientTable->hideColumn(0);
    ui->clientTable->setItemDelegateForColumn(2, new QSqlRelationalDelegate(ui->clientTable));
//    ui->clientTable->setItemDelegateForColumn(0, new DateEditDelegate(ui->clientTable));
    ui->clientTable->show();
}

MainWindow::~MainWindow()
{
    delete db;
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    TblModel * model = db->getModel();
    model->setDebug(true);
    model->addColumnData("add_date", QVariant(ui->dateAdd->dateTime()));
    model->addRelationalData(db->getModel()->fieldIndex("district"), ui->district->currentText());
    model->addColumnData("cert_id", QVariant(ui->idNumber->text().toInt()));
    model->addColumnData("name", QVariant(ui->name->text()));
    model->addColumnData("birth_date", QVariant(ui->birthDate->dateTime()));
    model->addColumnData("address", QVariant(ui->address->text()));
    model->addColumnData("phone", QVariant(ui->phoneNumber->text()));
    model->addRelationalData(db->getModel()->fieldIndex("diabetes_type"), ui->diabetesType->currentText());
    model->addRelationalData(db->getModel()->fieldIndex("disability_type"), ui->disabilityType->currentText());
    model->addColumnData("vtek_id", QVariant(ui->vtekNumber->text().toInt()));
    model->addColumnData("validity_date", QVariant(ui->validity->dateTime()));
    model->addColumnData("medicine",QVariant(ui->medicine->text()));
    QSqlError e = model->insert();
    //QComboBox * a;
    //a->itemDelegate()->setEditorData();
    qDebug() << "adding";
    if(e.number() != -1) {
        QMessageBox::warning(this, "Ошибка", QString("При добавлении произошла ошибка!\nТекст ошибки: \"")+e.text()+QString("\""));
    }
    else {
        QMessageBox::information(this, "Добавлено", "Добавление прошло успешно!");
    }
    qDebug() << "added";
    qDebug() << e.text();
}

void MainWindow::on_searchButton_clicked()
{
    TblModel * model = db->getModel();
    TblModel::Filter filter;
    filter.setFrom(ui->dateAddS0->dateTime().toTime_t());
    filter.setTo(ui->dateAddS1->dateTime().toTime_t());
    model->addFilter("add_date", filter);
    model->addFilter("address", TblModel::Filter(QVariant(ui->addressS->text())));
    model->addFilter("birth_date", TblModel::Filter(ui->birthDateS0->dateTime().toTime_t(), ui->birthDateS1->dateTime().toTime_t()));
    model->addFilter("diabetes_type", TblModel::Filter(QVariant(model->searchRelation(model->fieldIndex("diabetes_type"), ui->diabetesTypeS->currentText()))));
    model->addFilter("disability_type", TblModel::Filter(QVariant(model->searchRelation(model->fieldIndex("disability_type"), ui->disabilityTypeS->currentText()))));
    model->addFilter("district", TblModel::Filter(QVariant(model->searchRelation(model->fieldIndex("district"), ui->districtS->currentText()))));
    model->addFilter("cert_id", TblModel::Filter(ui->idNumberS0->text().toInt(), ui->idNumberS1->text().toInt()));
    model->addFilter("medicine", TblModel::Filter(QVariant(ui->medicineS->text())));
    model->addFilter("name", TblModel::Filter(QVariant(ui->nameS->text())));
    model->addFilter("phone", TblModel::Filter(QVariant(ui->phoneNumberS->text())));
    model->addFilter("validity_date", TblModel::Filter(ui->validityS0->dateTime().toTime_t(), ui->validityS1->dateTime().toTime_t()));
    model->addFilter("vtek_id", TblModel::Filter(ui->vtekNumberS0->text().toInt(), ui->vtekNumberS1->text().toInt()));
    model->createFilter();
    ui->exportButton->setDisabled(false);
}

void MainWindow::on_add_district_triggered()
{
    AddDialog * d = new AddDialog(this, District, this->db);
    if(d->exec()) {
        db->getModel()->relationModel(db->getModel()->fieldIndex("district"))->select();
    }
}

void MainWindow::on_add_diabetesType_triggered()
{
    AddDialog * d = new AddDialog(this, DiabetesType, this->db);
    if(d->exec()) {
        db->getModel()->relationModel(db->getModel()->fieldIndex("diabetes_type"))->select();
    }
}

void MainWindow::on_add_disabilityType_triggered()
{
    AddDialog * d = new AddDialog(this, DisabilityType, this->db);
    if(d->exec()) {
        db->getModel()->relationModel(db->getModel()->fieldIndex("disability_type"))->select();
    }
}

void MainWindow::on_edit_district_triggered()
{
    EditDialog * d = new EditDialog(this, "Редактирование списка районов", db->getModel()->relationModel(db->getModel()->fieldIndex("district")));
    d->exec();
}

void MainWindow::on_edit_diabetesType_triggered()
{
    EditDialog * d = new EditDialog(this, "Редактирование списка типов диабета", db->getModel()->relationModel(db->getModel()->fieldIndex("diabetes_type")));
    d->exec();
}

void MainWindow::on_edit_disabilityType_triggered()
{
    EditDialog * d = new EditDialog(this, "Редактирование списка типов инвалидности", db->getModel()->relationModel(db->getModel()->fieldIndex("disability_type")));
    d->exec();
}

void MainWindow::on_showAll_clicked()
{
    QString filter = this->db->getModel()->filter();
    this->db->getModel()->setFilter("");
    if(!this->db->getModel()->select()) {
        qDebug() << this->db->getModel()->lastError();
        qDebug() << "ERROR!!!!";
    }
    qDebug() << db->getModel()->query().executedQuery();
    ui->exportButton->setDisabled(false);
    //this->db->getModel()->setFilter(filter);
}

void MainWindow::on_exportButton_clicked()
{
    ExportDialog * d = new ExportDialog(this, db->getModel());
    d->exec();
}

void MainWindow::on_saveButton_clicked()
{
    if(!db->getModel()->submitAll()) {
        QMessageBox::warning(this, "Ошибка сохранения", "Во время сохранения возникла ошибка!");
    }
    else {
        QMessageBox::information(this, "Сохранено", "Сохранение прошло успешно");
    }
}
