//
// Created by viking on 9/16/18.
//

#ifndef SQL_SQLFOREIGNKEYDELEGATE_H
#define SQL_SQLFOREIGNKEYDELEGATE_H

#include "sqlsetup.h"
#include "columncombobox.h"
#include "Paper_Model.h"

class sqlForeignKeyDelegate: public QItemDelegate
        {
    Q_OBJECT
public:
    explicit sqlForeignKeyDelegate(Paper_Model *model_area,QObject *parent=Q_NULLPTR );                  // *areamodel is the model for the foreignkey
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    Paper_Model *model_area{nullptr};               // alias of foreign key model which contains the foreign key data
    QTableView *tableview{nullptr};                        //alias of table view (from *parent) allows resizing of table
};


#endif //SQL_SQLFOREIGNKEYDELEGATE_H
