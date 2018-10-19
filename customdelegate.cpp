#include "customdelegate.h"



#include <QKeyEvent>
#include <QStyledItemDelegate>
#include <QDoubleSpinBox>
#include <QTableView>
#include <QLineEdit>
//#include <QValidator>
#include <QRegExpValidator>

#include <QDebug>
customDelegate::customDelegate(QObject *parent) : QItemDelegate(parent)
{

}



QWidget *customDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    //    QLineEdit *editor = new QLineEdit(parent);
    //    QIntValidator *val = new QIntValidator(editor);

    //    val->setRange(0, 255);
    //    editor->setValidator(val);



//    QLineEdit * editor = new QLineEdit(parent);
////        editor->setValidator(new QValidator);
//    return editor;


//


    //^([A-Z]\\d{0}[a-z]* )+([A-Z]\\d{0}[a-z]*) *$

    QRegExp rx("^([A-Z]\\d{0}[a-z]*-?)+( [A-Z]\\d{0}[a-z]*)* *$" );
    QValidator *validator = new QRegExpValidator(rx, parent);

    QLineEdit *editor = new QLineEdit(parent);




    editor->setValidator(validator);
     return editor;

}

// this how the data gets from the model view architecture into the delegate itself
void customDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{

    //    double value = (index.model()->data(index,Qt::EditRole)).toDouble();

    //    QDoubleSpinBox * spinbox = static_cast<QDoubleSpinBox*>(editor);
    //    spinbox->setValue(value);

    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit * line = static_cast<QLineEdit*>(editor);
    value = value.trimmed();


    line->setText(value);





}


void customDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
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




void customDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    editor->setGeometry(option.rect);


}

//void customDelegate::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
//{
//    QTableView::closeEditor(editor, hint);
//    QTableView::closeEditor(nullptr, QAbstractItemDelegate::EditNextItem);
//}


//bool customDelegate::eventFilter(QObject *editor, QEvent* event)
//{
//    if (event->type()==QEvent::KeyPress)
//    {

//        QKeyEvent* key = static_cast<QKeyEvent*>(event);
//        if (key->key()==Qt::Key_Tab || key->key()==Qt::Key_Enter || key->key()==Qt::Key_Return)
//        {


//            //                   QWidget * edit = qobject_cast<QWidget*>(editor);

//            //                   emit commitData(edit);
//            //                   emit closeEditor(edit, QStyledItemDelegate::NoHint);

//            return false;

//        }
//        else
//        {
//            return QObject::eventFilter(editor, event);
//        }
//        return false;
//    }
//    else
//    {
//        return QObject::eventFilter(editor, event);
//    }
//    return false;









//}

