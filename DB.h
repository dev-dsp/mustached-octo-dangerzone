#ifndef DB_H
#define DB_H

#include <QtSql>
#include <QDebug>
#include "tblmodel.h"

class DB
{
public:
    DB(QObject * parent);
    ~DB();
    TblModel * getModel();
    QStringList getAllFromRelation(int columnIndex);
    void reset();
    static QSqlError deleteRelationRow(QSqlTableModel * model, int rowNumber);
    static int getIdByValue(QSqlTableModel * model, QString value);

private:
    void _prepareModel(QObject * parent);
    void _connectDB();
    QSqlDatabase _db;
    TblModel * model;
};

#endif // DB_H
