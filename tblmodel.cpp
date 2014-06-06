#include "tblmodel.h"

TblModel::TblModel(QObject *parent) :
    QSqlRelationalTableModel(parent)
{
    this->_needUpdate = true;
}
TblModel::TblModel(QObject *parent, QSqlDatabase db) :
    QSqlRelationalTableModel(parent, db)
{
    this->_needUpdate = true;
}

void TblModel::_setIndexes()
{
    for(int i=0; i<this->columnCount(); i++) {
        this->_indexes.push_back(this->headerData(i,Qt::Horizontal).toString());
    }
}

bool TblModel::setData(QModelIndex const &index, QVariant const &value, int role){
    qDebug() << "setData";
    if (!index.isValid() || role != Qt::EditRole){
        return false;
    }

    qDebug() << index << value << role;
    bool validEdit = false;
    if (index.column() == fieldIndex("add_date")){
        validEdit = QSqlRelationalTableModel::setData(index, value.toDateTime().toTime_t(), role);

        qDebug() << validEdit;
        if (validEdit){
            qDebug() << this->data(index);
            //emit dataChanged(index, index); // signal to the view that the item needs to be redrawn
            return true;
        }
    }
    validEdit = QSqlRelationalTableModel::setData(index, value, role);
    qDebug() << validEdit;
    return validEdit;
}

QVariant TblModel::data(const QModelIndex &item, int role) const{
    if(item.column() == this->fieldIndex("add_date") || item.column() == this->fieldIndex("birth_date") || item.column() == this->fieldIndex("validity_date")){
        if (role == Qt::TextAlignmentRole){
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        }

        QDateTime dateTime = QDateTime::fromTime_t(QSqlRelationalTableModel::data(item, role).toUInt());
        if (role == Qt::DisplayRole){
            return dateTime.toString("dd.MM.yyyy");
        }
        if (role == Qt::EditRole){
            return dateTime.date();
        }
    }
    return QSqlRelationalTableModel::data(item, role);
}

QSqlError TblModel::insert()
{
    if(this->_queryData.size()) {
        if(this->_needUpdate)
            this->_constructQuery();
        QSqlQuery q(this->database());
        if(this->_dbg){
            qDebug() << "TblModel::insert() " << this->_query;
        }
        if(q.exec(this->_query)) {
            return QSqlError();
        }
        else {
            return q.lastError();
        }
    }
    return QSqlError("Nothing to do", "Nothing to do");
}

int TblModel::fieldIndex(const QString &fieldName) const
{
    return this->_indexes.indexOf(fieldName);
}

QString TblModel::fieldIndex(const int i) const
{
    if(i < this->_indexes.size()) {
        return this->_indexes[i];
    }
    else {
        return QString();
    }
}

void TblModel::setDebug(bool set)
{
    this->_dbg = set;
}

void TblModel::addColumnData(QString columnName, QVariant data)
{
    this->_queryData.insert(columnName, data);
    this->_needUpdate = true;
}

QString TblModel::logInsertQuery()
{
    if(this->_needUpdate)
        this->_constructQuery();
    return this->_query;
}

void TblModel::createFilter()
{
    if(!this->_filter.size()) {
        return;
    }
    QString filter = "";
    foreach (QString column, this->_filter.keys()) {
        if(this->_filter[column].empty())
            qDebug() << "er";

        if(this->_filter[column].type() == TblModel::Filter::Ranged) {
            if(this->_filter[column].to() != NULL) {
                filter += column+QString(" <= ")+QString::number(this->_filter[column].to())+QString(" AND ");
            }
            if(this->_filter[column].from() != NULL) {
                filter += column+QString(" >= ")+QString::number(this->_filter[column].from())+QString(" AND ");
            }
        }
        else {
            if(QString(this->_filter[column].equal().typeName()) == QString("int")) {
                filter += column+QString(" = ")+QString::number(this->_filter[column].equal().toInt())+QString(" AND ");
            }
            else if(QString(this->_filter[column].equal().typeName()) == QString("QDateTime")) {
                filter += column+QString(" = ")+QString::number(this->_filter[column].equal().toDateTime().toTime_t())+QString(" AND ");
            }
            else {
                filter += QString("\"tbl_clients\".\"")+column+QString("\"")+QString(" LIKE ")+QString("'")+this->_filter[column].equal().toString()+QString("'")+QString(" AND ");
            }
        }
    }
    filter.remove(filter.length()-5, 5);
    qDebug() << filter;
    this->setFilter(filter);
    this->select();
    qDebug() << this->query().lastQuery();
}

void TblModel::addRelationalData(int relationID, QString data)
{
    QString tbl = this->relation(relationID).tableName();
    QString relCol = this->relation(relationID).displayColumn();
    QSqlQuery q(this->database());
    if(!q.exec(QString("SELECT `id` FROM ")+tbl+QString(" WHERE `")+relCol+QString("` = '")+data+QString("';"))) {
        //throw "ERROR!!!!!";
    }
    q.next();
    for(int i = 0; i<this->record().count(); i++){
        qDebug() << this->record().fieldName(i);
    }
    qDebug() << this->query().lastQuery();
    if(this->_dbg)
        qDebug() << this->fieldIndex(relationID) << "("<<relationID<<", in "<<tbl<<") -> [ "<<data<<" | "<<q.value(0).toInt()<<" ];";
    this->addColumnData(this->fieldIndex(relationID), QVariant(q.value(0).toInt()));
}

void TblModel::addFilter(QString columnName, TblModel::Filter data)
{
    if(!data.empty()) {
        this->_filter.insert(columnName, data);
    }
}

int TblModel::searchRelation(int relationID, QString value)
{
    QString tbl = this->relation(relationID).tableName();
    QString col = this->relation(relationID).displayColumn();
    QSqlQuery q(this->database());
    if(!q.exec(QString("SELECT `id`")+QString(" FROM ")+tbl+QString(" WHERE ")+col+QString("='")+value+QString("';"))) {
        throw "ERROR!!!!!!";
    }
    q.next();
    return q.value(0).toInt();
}

void TblModel::_constructQuery()
{
    QString names="", values="";
    //qDebug()<< this->_queryData;
    foreach (QString key, this->_queryData.keys()) {
        names+=QString("`")+key+QString("`,");
        if(QString(this->_queryData[key].typeName()) == QString("int")) {
            values+=QString::number(this->_queryData[key].toInt());
        }
        else if(QString(this->_queryData[key].typeName()) == QString("QDateTime")) {
            values+=QString::number(this->_queryData[key].toDateTime().toTime_t());
        }
        else {
            values+=QString("'")+this->_queryData[key].toString()+QString("'");
        }
        values+=QString(",");
    }
    names[names.length()-1] = ' ';
    values[values.length()-1] = ' ';
    this->_query = QString("INSERT INTO ")+this->tableName()+QString("(")+names+QString(") VALUES(")+values+QString(");");
    this->_needUpdate = false;
}

int TblModel::Filter::from() const
{
    return _from;
}

bool TblModel::Filter::empty()
{
    if(this->from() > this->to() && this->to() != NULL) {
        this->setType(TblModel::Filter::Empty);
    }
    if(this->type() == TblModel::Filter::Simple && QString(this->equal().typeName()) == QString("QString") && this->equal().toString().length() == 0) {
        this->setType(TblModel::Filter::Empty);
    }
    return this->_type == TblModel::Filter::Empty;
}

TblModel::Filter::Type TblModel::Filter::type() const
{
    return this->_type;
}

void TblModel::Filter::setFrom(int from)
{
    _from = from;
    if(from == this->to()) {
        this->_type = this->Empty;
    }
}

TblModel::Filter::Filter()
{
    this->_type = TblModel::Filter::Empty;
    this->_from = NULL;
    this->_to = NULL;
}

TblModel::Filter::Filter(QVariant data)
{
    this->_equal = data;
    this->_type = this->Simple;
    if(data.isNull() || !data.isValid()) {
        this->_type = this->Empty;
    }
    this->_from = NULL;
    this->_to = NULL;
}

TblModel::Filter::Filter(int from, int to)
{
    this->_from = from;
    this->_to = to;
    this->_type = this->Ranged;
    if(from == to) {
        this->_type = this->Empty;
    }
}

int TblModel::Filter::to() const
{
    return _to;
}

void TblModel::Filter::setTo(int to)
{
    _to = to;
    this->_type = this->Ranged;
    if(to == this->from()) {
        this->_type = this->Empty;
    }
}

QVariant TblModel::Filter::equal() const
{
    return _equal;
}

void TblModel::Filter::setType(TblModel::Filter::Type type)
{
    this->_type = type;
}

void TblModel::Filter::setEqual(const QVariant &equal)
{
    _equal = equal;
    this->_type = this->Simple;
    if(equal.isNull() || !equal.isValid()) {
        this->_type = this->Empty;
    }
}
