// delegate for SqllistView() for the foreign key column listing
// this delegate allows editing of the foreign key model and values
// this delegate is necessary to enable distinction between a user edit of the Paper_Model() and an edit resulting from and undo or redo operation
#ifndef SQLLISTDELEGATE_H
#define SQLLISTDELEGATE_H
#include "sqlsetup.h"


class sqlListDelegate: public QItemDelegate
{
    Q_OBJECT
public:
    explicit sqlListDelegate(QObject *parent=Q_NULLPTR);
    QLineEdit* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setModelData(QWidget *, QAbstractItemModel *, const QModelIndex &) const override;
    void setEditorData(QWidget *editor, const QModelIndex &) const override;        // overridden to disable default loading of model into editor on loss of focus on edited table cell
private slots:
    void commitAndCloseEditor();        // commit data and close editor on loss of focus in table cell


private:
    QListView *sqllistview{Q_NULLPTR};  // view of sql table
};

#endif // SQLLISTDELEGATE_H
