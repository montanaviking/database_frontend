//
// Created by viking on 8/26/18.
//
#include "SqlTableView.h"
#include "sqltabledelegate.h"
#include "constants.h"

SqlTableView::SqlTableView(QWidget *parent):QTableView(parent)
{
    setMouseTracking(true);     // needed to catch mouse moves
}

// redefined to enable setting of alias pmodel to right type
void SqlTableView::setModel(QAbstractItemModel *md)
{
    QTableView::setModel(md);
    pmodel=dynamic_cast<Paper_Model *>(model());           // alias of model
    if (!pmodel) // catch bad cast (this should never happen)
    {
        cout<<"ERROR! SqlTableView::setModel could not cast"<<endl;
        exit(1);
    }
}





void SqlTableView::keyPressEvent(QKeyEvent *ke)
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





void SqlTableView::mousePressEvent(QMouseEvent *me)
{
    auto selectedindex = indexAt(me->pos());
    if(me->button()==Qt::RightButton && selectedindex.isValid())       // then bring up menu
    {
        cout<<"row selected "<<selectedindex.row()<<endl;
        QMenu editmenu(this);       // popup menu to add row, delete row, or copy row
        QAction *insertrow = new QAction("insert new row",&editmenu);
        //QAction *copyrow = new QAction("copy row",&editmenu);
        QAction *deleterow = new QAction("delete row",&editmenu);
        deleterow->setData(selectedindex.row());        // inform the delete row action of the row index to delete which is selectedindex.row()
        editmenu.addAction(insertrow);
        //editmenu.addAction(copyrow);
        editmenu.addAction(deleterow);
        if(selectedindex.column()==LINKCOL)                        // did user select a cell within the link column?
        {
            //cout<<"selected column "<<selectedindex.column()<<endl;
            if(pmodel->getcelldata(selectedindex).toString()!="")               // does the link contain a filename?
            {
                QAction *openlinkedfile = new QAction("open file",&editmenu);       // add menu item to follow link to open file
                openlinkedfile->setData(pmodel->getcelldata(selectedindex).toString());         // inform action item openlinkedfile as to the file which should be opened
                editmenu.addAction(openlinkedfile);
                connect(openlinkedfile,SIGNAL(triggered()),this,SLOT(actionslot_openlinkedfile()));             // connect to slot which opens the file fromt the selected link
            }
            QAction *setlink = new QAction("set link to file",&editmenu);       // in the link column, add menu item to allow setting link to a selected file
            setlink->setData(QVariant(selectedindex));                          // send selected index to set the link
            editmenu.addAction(setlink);
            connect(setlink,SIGNAL(triggered()),this,SLOT(actionslot_setlink()));
        }
        connect(deleterow,SIGNAL(triggered()),this,SLOT(actionslot_rowdelete()));
        connect(insertrow,SIGNAL(triggered()),this,SLOT(actionslot_rowinsert()));

        editmenu.exec(me->globalPos());
    }
   else
    {
        QTableView::mousePressEvent(me);        // default mouse action for left mouse click
    }
}

// catch mouse hovers and highlight row which is under the mouse
// the row under the mouse could be acted on according to the user
void SqlTableView::mouseMoveEvent(QMouseEvent *me)
{
    QModelIndex index = indexAt(me->pos());
    //cout<<"from line 64 in SqlTableView.cpp row selected  "<<index.row()<<endl;
    //pmodel->hoverrow=index.row();        // change background color on hovered row to indicate row which might be changed
    selectRow(index.row());
    pmodel->setData(index,0,Qt::BackgroundColorRole);   // this is needed to force update of table colors the value is a bogus value and not used
}



// slots for actions in popups

// delete row action slot
void SqlTableView::actionslot_rowdelete()
{
    QAction *act = dynamic_cast<QAction *>(sender());               // get QAction pointer which called this slot
    if(act==nullptr)
    {
        cout<<"ERROR in SqlTableView actionslot_rowdelete() unable to cast pointer to sender action"<<endl;
        exit(1);
    }
    int rownum = act->data().toInt();                   // get data, i.e. row number from QAction which called this slot
    pmodel->deleterow(rownum);          // set row in the table model which is to be deleted upon next submission to database
    hideRow(rownum);        // make to-be-deleted row invisible because pmodel->deleterow() does not immediately delete the row from the model and
                            // the row would still be visible. The row to be deleted will finally be deleted from pmodel (the main table model) and
                            // also the underlying database upon the user driven manual submission (save button)
    //cout<<"from line 90 in SqlTableView.cpp action parameter = "<<rownum<<endl;
}



// insert blank row action slot
void SqlTableView::actionslot_rowinsert()
{
    pmodel->insertrow();
    cout<<"from line 79 in SqlTableView.cpp action rowadd"<<endl;
}



// open selected file from link in cell
void SqlTableView::actionslot_openlinkedfile()
{
    QAction *act = dynamic_cast<QAction *>(sender());       // get QAction which called this slot
    if(act==nullptr)
    {
        cout<<"ERROR in SqlTableView actionslot_openlinkedfile() unable to cast pointer to sender action"<<endl;
        exit(1);
    }
    QString fullfilename=DATAROOTDIRECTORY+"/d"+act->data().toString();
    cout<<"selected file is "<<fullfilename.toStdString()<<endl;
    auto filecheck = QFile(fullfilename);
    if(!filecheck.exists())         // does the selected file exist?
    {
        auto errmsg = QMessageBox();
        errmsg.setText("ERROR: file "+fullfilename+" not found");
        errmsg.exec();
        return;
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(fullfilename));
    return;
}


// set link to file selected from menu
void SqlTableView::actionslot_setlink()
{
    QStringList selectedfiles;
    QAction *act = dynamic_cast<QAction *>(sender());       // get QAction which called this slot
    if(act==nullptr)
    {
        cout<<"ERROR in SqlTableView actionslot_setlink() unable to cast pointer to sender action"<<endl;
        exit(1);
    }
    auto linkindex=act->data().toModelIndex();              // get index of table cell to place link into

    cout<<"from line 169 in SqlTableView.cpp set link row = "<<linkindex.row()<<" set link column = "<<linkindex.column()<<endl;      // debug to test calling of this method
    // now allow user to browse for file to link to
    auto fd = QFileDialog(this,"Library Files ","/home/viking/library/downloadpaper" );
    fd.setOption(QFileDialog::ReadOnly,true);
    fd.setFileMode(QFileDialog::ExistingFile);
    if(fd.exec())
    {
        selectedfiles=fd.selectedFiles();
    }
    if(selectedfiles.size()>0)                  // did the user actually make a file selection to link?
    {
        auto qfi = QFileInfo(selectedfiles[0]);
        auto selectedfile=qfi.fileName().remove(0,1);                   // remove leading character "d" from string to maintain compatibility with phpmyadmin setup
        cout<<"selected file"<<selectedfile.toStdString()<<endl;
        pmodel->setData(linkindex,QVariant::fromValue(selectedfile),useredit);          // set model cell to the user-selected link. Note role=useredit which means this is a manual user edit
    }
}



// undo all edits and reload model from database
void SqlTableView::undo_all_edits()
{
    pmodel->undo_all_edits();                   // re-populate model from database
    for(int i=0;i<pmodel->rowCount();i++)       // unhide all rows in view
    {
        setRowHidden(i,false);
    }
}
