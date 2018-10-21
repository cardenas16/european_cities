#ifndef CUSTOMDELEGATE_H
#define CUSTOMDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QSize>
#include <QObject>


class customDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit customDelegate(QObject *parent = nullptr);

    QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void	setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;
    void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const;
//    void closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint);
    bool eventFilter(QObject *editor, QEvent* event);



signals:

public slots:
};

#endif // CUSTOMDELEGATE_H
