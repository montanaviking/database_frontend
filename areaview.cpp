#include "areaview.h"
#include "constants.h"

areaView::areaView(QWidget *parent):QListView(parent)
{
    setMouseTracking(true);     // needed to catch mouse moves
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
}



void areaView::setModel(QAbstractItemModel *md)
{
    QListView::setModel(md);
    setModelColumn(0);
    pmodel=dynamic_cast<Paper_Model *>(model());           // alias of model
    if (!pmodel) // catch bad cast (this should never happen)
    {
        cout<<"ERROR! areaView::setModel could not cast"<<endl;
        exit(1);
    }

}






void areaView::keyPressEvent(QKeyEvent *ke)
{
    int selectedrow = selectionModel()->currentIndex().row();       // get selected row
    if(selectionModel()->currentIndex().isValid() && ke->modifiers()==Qt::ControlModifier)
    {
        if(ke->key()==Qt::Key_R)    // user hit cntl-r
        {
            pmodel->undo_redo(redo,selectedrow);
        }
        else if(ke->key()==Qt::Key_Z) // user hit cntl-z
        {
            pmodel->undo_redo(undo,selectedrow);
        }
    }
}



// form up menu to insert, delete, and copy foreign key items
void areaView::mousePressEvent(QMouseEvent *me)
{
    auto selectedindex = indexAt(me->pos());
    if(me->button()==Qt::RightButton && selectedindex.isValid())       // then bring up menu
    {
        cout<<"from line 55 in areaview.cpp row selected "<<selectedindex.row()<<endl;
        QMenu editmenu(this);       // popup menu to add row, delete row, or copy row
        QAction *insertrow = new QAction("insert new row",&editmenu);
        QAction *copyrow = new QAction("copy row",&editmenu);
        QAction *deleterow = new QAction("delete row",&editmenu);
        deleterow->setData(selectedindex.row());
        editmenu.addAction(insertrow);
        editmenu.addAction(copyrow);                // not implemented yet
        editmenu.addAction(deleterow);
        connect(deleterow,SIGNAL(triggered()),this,SLOT(actionslot_rowdelete()));
        connect(insertrow,SIGNAL(triggered()),this,SLOT(actionslot_rowinsert()));
        editmenu.exec(me->globalPos());
    }
   else
    {
        QListView::mousePressEvent(me);        // default mouse action for left mouse click
    }
}



// catch mouse hovers and highlight row which is under the mouse
// the row under the mouse could be acted on according to the user
void areaView::mouseMoveEvent(QMouseEvent *me)
{
    QModelIndex index = indexAt(me->pos());
    //cout<<"from line 64 in SqlTableView.cpp row selected  "<<index.row()<<endl;
    //pmodel->hoverrow=index.row();        // change background color on hovered row to indicate row which might be changed
    setSelection(rectForIndex(index),QItemSelectionModel::ClearAndSelect);
    pmodel->setData(index,0,Qt::BackgroundColorRole);   // this is needed to force update of table colors. The value is a bogus value and not used since we are updating the background color only
}



// slots for actions in popups
// delete row action slot
void areaView::actionslot_rowdelete()
{
    QAction *act = dynamic_cast<QAction *>(sender());
    if(act==nullptr)
    {
        cout<<"ERROR in SqlTableView actionslot_rowdelete() unable to cast pointer to sender action"<<endl;
        exit(1);
    }
    int rownum = act->data().toInt();
    pmodel->deleterow(rownum);          // set row which is to be deleted upon next submission to database
    setRowHidden(rownum,true);        // make to-be-deleted row invisible
    cout<<"from line 102 in SqlTableView.cpp action parameter = "<<rownum<<endl;
}



// insert blank row action slot
void areaView::actionslot_rowinsert()
{
    pmodel->insertrow();
    cout<<"from line 111 in SqlTableView.cpp action rowadd"<<endl;
}

