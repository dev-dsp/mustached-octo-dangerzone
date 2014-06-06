#include "DB.h"

DB::DB(QObject * parent)
{
    this->_connectDB();
    this->_prepareModel(parent);
}

DB::~DB()
{
    _db.close();
}

void DB::_prepareModel(QObject * parent)
{
    model = new TblModel(parent, _db);
    model->setTable(_db.driver()->escapeIdentifier("tbl_clients", QSqlDriver::TableName));
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
//    model->removeColumn(0);
    model->_setIndexes();
    model->setHeaderData(1, Qt::Horizontal, "Дата\nдобавления");
    model->setHeaderData(2, Qt::Horizontal, "Район");
    model->setHeaderData(3, Qt::Horizontal, "Номер\nудостоверения");
    model->setHeaderData(4, Qt::Horizontal, "ФИО");
    model->setHeaderData(5, Qt::Horizontal, "Дата\nрождения");
    model->setHeaderData(6, Qt::Horizontal, "Адрес");
    model->setHeaderData(7, Qt::Horizontal, "Номер\nтелефона");
    model->setHeaderData(8, Qt::Horizontal, "Тип\nдиабета");
    model->setHeaderData(9, Qt::Horizontal, "Инвалидность");
    model->setHeaderData(10, Qt::Horizontal, "Номер\nВТЭК");
    model->setHeaderData(11, Qt::Horizontal, "Срок\nдействия");
    model->setHeaderData(12, Qt::Horizontal, "Лекарство");
    this->reset();
}


void DB::_connectDB()
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName("./.db");
    if (!_db.open() || !_db.isOpen()) {
        qDebug() << _db.lastError().text();
    }
    QSqlQuery qcreate(_db);
    if(!qcreate.exec(QString(QString("CREATE TABLE IF NOT EXISTS 'tbl_clients' (")+
                    QString("             'id' INTEGER PRIMARY KEY AUTOINCREMENT,")+
                    QString("             'add_date' INTEGER,")+
                    QString("             'district' INTEGER,")+
                    QString("             'cert_id' INTEGER,")+
                    QString("             'name' TEXT,")+
                    QString("             'birth_date' INTEGER,")+
                    QString("             'address' TEXT,")+
                    QString("             'phone' TEXT,")+
                    QString("             'diabetes_type' INTEGER,")+
                    QString("             'disability_type' INTEGER,")+
                    QString("             'vtek_id' INTEGER,")+
                    QString("             'validity_date' INTEGER,")+
                    QString("             'medicine' TEXT")+
                    QString("        )")))) {
        qDebug() << "verybad...";
    }
    if(!qcreate.exec(QString(QString("CREATE TABLE IF NOT EXISTS 'tbl_diabetes_types' (")+
                    QString("             'id' INTEGER PRIMARY KEY AUTOINCREMENT,")+
                    QString("             'type' TEXT,")+
                    QString("             'deleted' INTEGER DEFAULT 0")+
                    QString(")"))))
                     {
        qDebug() << "verybad...";
    }
    if(!qcreate.exec(QString(QString("CREATE TABLE IF NOT EXISTS 'tbl_districts' (")+
                    QString("             'id' INTEGER PRIMARY KEY AUTOINCREMENT,")+
                    QString("             'name' TEXT,")+
                    QString("             'deleted' INTEGER DEFAULT 0")+
                    QString(")")))) {
        qDebug() << "verybad...";
    }
    if(!qcreate.exec(QString(QString("CREATE TABLE IF NOT EXISTS 'tbl_disability_types' (")+
                    QString("             'id' INTEGER PRIMARY KEY AUTOINCREMENT,")+
                    QString("             'type' TEXT,")+
                    QString("             'deleted' INTEGER DEFAULT 0")+
                    QString(")")))) {
        qDebug() << "verybad...";
    }
}

TblModel * DB::getModel()
{
    return model;
}

void DB::reset()
{
    model->setRelation(model->fieldIndex("district"), QSqlRelation(_db.driver()->escapeIdentifier("tbl_districts", QSqlDriver::TableName),"id","name"));
    model->setRelation(model->fieldIndex("diabetes_type"), QSqlRelation(_db.driver()->escapeIdentifier("tbl_diabetes_types", QSqlDriver::TableName),"id","type"));
    model->setRelation(model->fieldIndex("disability_type"), QSqlRelation(_db.driver()->escapeIdentifier("tbl_disability_types", QSqlDriver::TableName),"id","type"));
}

QSqlError DB::deleteRelationRow(QSqlTableModel * model, int rowNumber)
{
    QString tbl = model->tableName();
    QSqlQuery q(model->database());
    q.exec(QString("UPDATE ")+tbl+QString(" SET `deleted` = 1 WHERE `id` = ")+QString::number(rowNumber)+QString(";"));
    return q.lastError();
}
