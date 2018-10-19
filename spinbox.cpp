#include "spinbox.h"


#include <QKeyEvent>
#include <QStyledItemDelegate>
#include <QDoubleSpinBox>
#include <QDebug>



SpinBox::SpinBox(QObject *parent) : QItemDelegate(parent)
{


}



QWidget * SpinBox::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{

        QDoubleSpinBox * editor = new QDoubleSpinBox(parent);

        editor->setMinimum(0);
        editor->setMaximum(100);
        return editor;


}
// // this how the data gets from the model view architecture into the delegate itself
void SpinBox::setEditorData(QWidget * editor, const QModelIndex & index) const
{

      double value = (index.model()->data(index,Qt::EditRole)).toDouble();

      QDoubleSpinBox * spinbox = static_cast<QDoubleSpinBox*>(editor);
      spinbox->setValue(value);


}


void SpinBox::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{

    QDoubleSpinBox * spinbox = static_cast<QDoubleSpinBox*>(editor);
    spinbox->interpretText();
    double value = spinbox->value();
    model->setData(index,value, Qt::EditRole);

}
void SpinBox::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    editor->setGeometry(option.rect);


}


bool SpinBox::eventFilter(QObject *editor, QEvent* event)
{
        if (event->type()==QEvent::KeyPress)
           {
               QKeyEvent* key = static_cast<QKeyEvent*>(event);
               if (key->key()==Qt::Key_Tab || key->key()==Qt::Key_Enter || key->key()==Qt::Key_Return)
               {
    //               QLineEdit *editor=qobject_cast<QLineEdit*>(obj);


                     qDebug() << " tab/enter/return has been hit";
    //               emit commitData(editor);
    //               emit closeEditor(editor, QStyledItemDelegate::NoHint);
                      return true;

               }
               else
               {
                   return QObject::eventFilter(editor, event);
               }
               return false;
           }
           else
           {
               return QObject::eventFilter(editor, event);
           }
           return false;



}

