#ifndef TBLMODEL_H
#define TBLMODEL_H

#include <QSqlRelationalTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QSqlField>

class TblModel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    class Filter {
    public:
        enum Type {
            Ranged,
            Simple,
            Empty
        };

        Filter();
        Filter(QVariant data);
        Filter(int from, int to);

        int to() const;
        int from() const;
        bool empty();
        Type type() const;
        void setTo(int to);
        void setFrom(int from);
        QVariant equal() const;
        void setEqual(const QVariant &equal);

    private:
        void setType(TblModel::Filter::Type type);
        int _to;
        int _from;
        QVariant _equal;
        Type _type;
    };
public:
    void addFilter(QString columnName, TblModel::Filter data);
    int searchRelation(int relationID, QString value);
    void addColumnData(QString columnName, QVariant data);
    void addRelationalData(int relationID, QString data);
    explicit TblModel(QObject *parent = 0);
    QString logInsertQuery();
    void createFilter();
    void setDebug(bool);
    QSqlError insert();
    int fieldIndex(const QString &fieldName) const;
    QString fieldIndex(const int i) const;
    void _setIndexes();
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;

    explicit TblModel(QObject *parent, QSqlDatabase db);
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
signals:

public slots:

private:
    bool _dbg;
    QString _query;
    QMap<QString, QVariant> _queryData;
    bool _needUpdate;
    void _constructQuery();
    QMap<QString, TblModel::Filter> _filter;
    QVector<QString> _indexes;

};

#endif // TBLMODEL_H
