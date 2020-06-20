// delegate for SqlTableView()
// this delegate is necessary to enable distinction between a user edit of the Paper_Model() and an edit resulting from and undo or redo operation
#ifndef SQLTABLEDELEGATE_H
#define SQLTABLEDELEGATE_H
#include "sqlsetup.h"
//#include <QRgba64>
//#include <QBrush>
//#include <QObject>


//using namespace Qt;
//template<typename T>
// T is QTableView or QListView
class sqlTableDelegate: public QItemDelegate
{
    Q_OBJECT
public:
    explicit sqlTableDelegate(QObject *parent=Q_NULLPTR);
    QLineEdit* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setModelData(QWidget *, QAbstractItemModel *, const QModelIndex &) const override;
    void setEditorData(QWidget *editor, const QModelIndex &) const override;        // overridden to disable default loading of model into editor on loss of focus on edited table cell
private slots:
    void commitAndCloseEditor();        // commit data and close editor on loss of focus in table cell


private:
    QTableView *sqltableview{nullptr};  // view of sql table
    //bool editorEvent(QEvent *ee, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    //int hoverrow{-1};
    //QBrush hoverbrush(QColor(lightblue),1);
};

#endif // SQLTABLEDELEGATE_H
