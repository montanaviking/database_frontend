#include "sqltabledelegate.h"
#include<sqlsetup.h>
using namespace std;

sqlTableDelegate::sqlTableDelegate(QObject *parent): QItemDelegate(parent),sqltableview(dynamic_cast<QTableView *>(parent))
{

     //connect(parent,SIGNAL(signalhoverindexchanged(QModelIndex &)),this,SLOT(setdata_on_mouse_move(QModelIndex &)));
}

QLineEdit* sqlTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto *editor=new QLineEdit(parent);
    cout<<"from line 15 in sqlTableDelegate::createEditor"<<endl;
    auto sqltablemodel = index.model();
    editor->setText(sqltablemodel->data(index).toString());
    editor->setReadOnly(false);             //allow editing
    connect(editor,SIGNAL(editingFinished()),this,SLOT(commitAndCloseEditor()));
    return editor;
}


// reimplemented as a do-nothing method to disable automatic loading of the editor text from the model into the table when editor focus is lost
// now editor text data are loaded from the database just once on editor creation
void sqlTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{}




void sqlTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineedit = dynamic_cast<QLineEdit *>(editor);
    if(lineedit==nullptr)       // catch failed dynamic cast, should never happen diagnostics only
    {
        cout<<"ERROR line 35 in sqltabledelegate.cpp lineedit dynamic cast failed"<<endl;
        exit(1);
    }
    if(index.isValid())
    {
        model->setData(index,lineedit->text(),useredit); // The key point of this delegate is to set an edit role, i.e. useredit, which indicates a direct user edit to the sqltablemodel (table) as opposed to a programmatic edit
        //qDebug()<<"from line 41 in sqltabledelegate.cpp: setData for sqltablemodel set to"<<lineedit->text()<<"model data are "<<model->data(index,Qt::DisplayRole).toString();
        emit model->dataChanged(index,index);
        sqltableview->setWordWrap(false);
        sqltableview->resizeRowsToContents();        // resize row to reflect any changes to its data
        sqltableview->resizeColumnsToContents();        // resize row to reflect any changes to its data
    }
    else
    {
        QItemDelegate::setModelData(editor,model,index);
    }
}



// commit data to model and close editor on loss of focus (movement of mouse out of edited cell)
void sqlTableDelegate::commitAndCloseEditor()
{
    QLineEdit *editor = dynamic_cast<QLineEdit *>(sender());
    cout<<"from line 59 in sqllistdelegate::commitAndCloseEditor "<<editor->text().toStdString()<<endl;
    emit commitData(editor);
    emit closeEditor(editor);
}

