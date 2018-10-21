#ifndef SPINBOX_H
#define SPINBOX_H


#include <QItemDelegate>
#include <QModelIndex>
#include <QSize>
#include <QObject>


class SpinBox : public QItemDelegate
{
    Q_OBJECT
public:
    explicit SpinBox(QObject *parent = nullptr);


    QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void	setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;
    void	updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    bool eventFilter(QObject *editor, QEvent* event);

signals:

public slots:
};

#endif // SPINBOX_H
