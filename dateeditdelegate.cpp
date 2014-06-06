#include "dateeditdelegate.h"

DateEditDelegate::DateEditDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *DateEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDateEdit * editor = new QDateEdit(parent);
    return editor;
}

void DateEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDateEdit * dateEdit = static_cast<QDateEdit*>(editor);
    dateEdit->setDate(index.model()->data(index).toDate());
}

void DateEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateEdit * dateEdit = static_cast<QDateEdit*>(editor);
    model->setData(index, dateEdit->dateTime().toTime_t());
}

void DateEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
