#ifndef SPINBOXDELEGATE_H
#define SPINBOXDELEGATE_H


#include <QItemDelegate>
#include <QModelIndex>
#include <QSize>
#include <QObject>


class SpinBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit SpinBoxDelegate(QObject *parent = nullptr);


    QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void	setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;
    void	updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    bool eventFilter(QObject *editor, QEvent* event);

signals:

public slots:
};

#endif // SPINBOXDELEGATE_H
