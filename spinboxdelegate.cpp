#include "spinboxdelegate.h"


#include <QKeyEvent>
#include <QStyledItemDelegate>
#include <QDoubleSpinBox>
#include <QDebug>



SpinBoxDelegate::SpinBoxDelegate(QObject *parent) : QItemDelegate(parent)
{


}



QWidget * SpinBoxDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{

        QDoubleSpinBox * editor = new QDoubleSpinBox(parent);

        editor->setMinimum(0);
        editor->setMaximum(100);
        return editor;


}
// // this how the data gets from the model view architecture into the delegate itself
void SpinBoxDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{

      double value = (index.model()->data(index,Qt::EditRole)).toDouble();

      QDoubleSpinBox * spinbox = static_cast<QDoubleSpinBox*>(editor);
      spinbox->setValue(value);


}


void SpinBoxDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{

    QDoubleSpinBox * spinbox = static_cast<QDoubleSpinBox*>(editor);
    spinbox->interpretText();
    double value = spinbox->value();
    model->setData(index,value, Qt::EditRole);

}
void SpinBoxDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    editor->setGeometry(option.rect);


}


bool SpinBoxDelegate::eventFilter(QObject *editor, QEvent* event)
{
        if (event->type()==QEvent::KeyPress)
           {
               QKeyEvent* key = static_cast<QKeyEvent*>(event);
               if (key->key()==Qt::Key_Tab || key->key()==Qt::Key_Enter || key->key()==Qt::Key_Return)
               {

                   QDoubleSpinBox * value = qobject_cast<QDoubleSpinBox*>(editor);

                      emit commitData(value);
                      emit closeEditor(value, QStyledItemDelegate::NoHint);


                      return true;
               }

           }
           else
           {
               return QObject::eventFilter(editor, event);
           }
           return false;







}

