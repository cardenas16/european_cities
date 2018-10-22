#include "newitemdelegate.h"





#include <QKeyEvent>
#include <QStyledItemDelegate>
#include <QDoubleSpinBox>
#include <QTableView>
#include <QLineEdit>
#include <QRegExpValidator>

#include <QDebug>



NewItemDelegate::NewItemDelegate(QObject *parent) : QItemDelegate(parent)
{

}


QWidget *NewItemDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    //^([A-Z]\\d{0}[a-z]*-?)+( [A-Z]\\d{0}[a-z]*)* *$

    // ^([A-Z]\\d{0}[a-z]*-? )+([A-Z]\\d{0}[a-z]*) *$ // works fine


    //
    QRegExp rx("^([A-Z]\\d{0}[a-z]*-?[a-z]* )+([A-Z]\\d{0}[a-z]*) *$" );
    QValidator *validator = new QRegExpValidator(rx, parent);

    QLineEdit *editor = new QLineEdit(parent);




    editor->setValidator(validator);
    return editor;


}

// this how the data gets from the model view architecture into the delegate itself
void NewItemDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{

    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit * line = static_cast<QLineEdit*>(editor);
    value = value.trimmed();


    line->setText(value);


}


void NewItemDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{

    //    QDoubleSpinBox * spinbox = static_cast<QDoubleSpinBox*>(editor);
    //    spinbox->interpretText();
    //    double value = spinbox->value();
    //    model->setData(index,value, Qt::EditRole);




    QLineEdit *line = static_cast<QLineEdit*>(editor);
    QString value = line->text();
    value = value.trimmed();


    model->setData(index, value);






}




void NewItemDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    editor->setGeometry(option.rect);


}

//void NewItemDelegate::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
//{
//    emit closeEditor(editor, hint);
//    emit closeEditor(nullptr, QAbstractItemDelegate::EditNextItem);
//}


bool NewItemDelegate::eventFilter(QObject *editor, QEvent* event)
{
    if (event->type()==QEvent::KeyPress)
    {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        if (key->key()==Qt::Key_Tab || key->key()==Qt::Key_Enter || key->key()==Qt::Key_Return )
        {


            QLineEdit *line = static_cast<QLineEdit*>(editor);
            QString value = line->text();
            value = value.trimmed();

            emit commitData(line);
            emit closeEditor(line, QStyledItemDelegate::NoHint);

            return false;

        }

    }
    else
    {
        return QObject::eventFilter(editor, event);
    }
    return false;



}

