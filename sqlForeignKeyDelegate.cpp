//
// Created by viking on 9/16/18.
//
#include "sqlsetup.h"
#include "sqlForeignKeyDelegate.h"
#include "Paper_Model.h"
#include "columncombobox.h"
// this delegate allows modification of main table columns whicha represent the foreign keys and allows the user to select one of the foreign key values from a list, to use to set a foreign key value for the
// selected record
sqlForeignKeyDelegate::sqlForeignKeyDelegate(Paper_Model *model_area, QObject *parent):QItemDelegate(parent),model_area(model_area),tableview(dynamic_cast<QTableView *>(parent)){}     // parent is the area table model
QWidget* sqlForeignKeyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto *editor=new ColumnComboBox(parent);
    editor->setModelColumn(0);        // set editor (Qcombobox) to model of area table
    editor->setModel(model_area);     // use foreign key values fromt the foreign key model (which is model_area)
    editor->setEditable(false);         // set editor to be read only so that foreign key MAY NOT be changed from the editor
    editor->setMaxVisibleItems(editor->count());    // allow the user to select from a list of all the possible foreign key values
    editor->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    //qDebug()<<"from line 26 sqlForeignKeyDelegate.cpp number of columns "<<editor->count();
    return editor;
}
void sqlForeignKeyDelegate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    auto comboboxdata = dynamic_cast<ColumnComboBox*>(editor);
    //qDebug()<<"from line 32 in sqlForeignKeyDelegate.cpp row,col = "<<comboboxdata->count();
    if (index.isValid())
    {
        //auto modeldata=comboboxdata->currentText();
        model->setData(index,comboboxdata->currentText(),useredit);  // this is a manual user edit, we need to distinguish a manual user edit from other internal edits for the Paper_Model() class such as undos and redos
        emit model->dataChanged(index,index);
        tableview->setWordWrap(false);
        tableview->resizeRowsToContents();        // resize row to reflect any changes to its data
        tableview->resizeColumnsToContents();        // resize row to reflect any changes to its data
    }
    else
    {
        QItemDelegate::setModelData(editor,model,index);
    }
}
