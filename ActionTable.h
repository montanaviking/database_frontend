//
// Created by viking on 8/12/18.
//
//

#ifndef SQL_ACTIONTABLE_H
#define SQL_ACTIONTABLE_H

#include <sqlsetup.h>
#include <ui_library.h>
#include "Paper_Model.h"
#include "SqlTableView.h"
#include <actionarealist.h>
#include "constants.h"


class ActionTable:public QDialog,public Ui_Sqltable
{
    Q_OBJECT
    public:
    explicit ActionTable(QString databasename,QString hostname=QString("Sibyl"),QString username=QString("montanaviking"),QString password=QString("nova"),QDialog *parent=Q_NULLPTR);
    QSqlError query_papers_table(QString filter=QString("1")) const;
    QSqlDatabase db;
    private:
    const QString hostname;
    const QString databasename;
    const QString username;
    const QString password;
    Paper_Model *model_papers{nullptr};         // model for main table
    Paper_Model *model_area{nullptr};           // model for foreign keys
    SqlTableView *sqlTableView{nullptr};        // view of main table
};

#endif //SQL_ACTIONTABLE_H
