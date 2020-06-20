//
// Created by viking on 8/12/18.
//

#include "ActionTable.h"
//#ifndef SQL_SQLSETUP_H
//#include <QDialog>
//#include <QtSql/QSqlDatabase>
//#endif
#include "Paper_Model.h"
#include "sqlForeignKeyDelegate.h"

#include "sqltablehheader.h"
#include "sqltabledelegate.h"

ActionTable::ActionTable(const QString databasename, const QString hostname,const QString username,const QString password, QDialog *parent):QDialog(parent)
{
    setupUi(this);
    db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(hostname);
    db.setDatabaseName(databasename);
    db.setUserName(username);
    db.setPassword(password);
    bool ok=db.open();
    // can we open this database?
    if(!ok) {
        QMessageBox mess;
        auto errormessage=QString("ERROR,CANNOT open database error ")+QString(db.lastError().text());
        mess.setText(errormessage);
        mess.exec();
        exit(1);
    }
    sqlTableView = new SqlTableView(this);
    verticalLayout->addWidget(sqlTableView);
    model_papers = new Paper_Model(db,this,"papers");       // papers table model, do not populate on this instantiation
    model_area = new Paper_Model(db,this,"area","1");       // area table model, populate with query for all database rows
    sqlTableHHeader *paperheaderview=new sqlTableHHeader(this,model_papers,model_area);   // horizontal headers
    sqlTableView->setModel(model_papers);
    sqlTableView->setHorizontalHeader(dynamic_cast<QHeaderView *>(paperheaderview));

    auto *delegate_area=new sqlForeignKeyDelegate(model_area,sqlTableView);    // data from model_area, parent = this delegate controls the behavior, allowing entry of area column data
    auto *delegate_papertable = new sqlTableDelegate(sqlTableView);
    sqlTableView->setItemDelegate(delegate_papertable);
    sqlTableView->setItemDelegateForColumn(AREACOL,dynamic_cast<QAbstractItemDelegate *>(delegate_area));
    sqlTableView->setItemDelegateForColumn(AREA2COL,dynamic_cast<QAbstractItemDelegate *>(delegate_area));
    sqlTableView->horizontalHeader()->setStretchLastSection(true);
    // buttons
    connect(undo_all_edits_button,SIGNAL(clicked()),sqlTableView,SLOT(undo_all_edits()));
    connect(save_button,SIGNAL(clicked()),model_papers,SLOT(save_model_to_database()));        // save model to database
}

// database query for main table
QSqlError ActionTable::query_papers_table(const QString filter) const
{
    auto sqlerror=model_papers->query(filter);
    sqlTableView->resizeColumnsToContents();
    sqlTableView->horizontalHeader()->setStretchLastSection(true);
    sqlTableView->hideColumn(0);
    sqlTableView->resizeColumnsToContents();
    sqlTableView->resizeRowsToContents();
    return sqlerror;
}

