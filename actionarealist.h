//
//Created by viking on Oct 27, 2019
// updated
// main menu to view area. Area is a foreign key of the main table and this view enables the users to
// display and update the list representing the area foreign keys of the main table

#ifndef ACTIONAREALIST_H
#define ACTIONAREALIST_H
#include <ui_area_editor.h>
#include <sqlsetup.h>
#include <Paper_Model.h>
#include <areaview.h>
#include <SqlTableView.h>
#include <sqllistdelegate.h>

class ActionAreaList:public QDialog, Ui_area_Editor
{
    Q_OBJECT
public:
    explicit ActionAreaList(QDialog *parent=Q_NULLPTR, Paper_Model *areamodel=Q_NULLPTR );
    QSqlError query_area_table(QString filter=QString("1")) const;      // database query
    QPointer<Paper_Model> areamodel{Q_NULLPTR};             // alias of area model
    QPointer<areaView> sqlAreaView{Q_NULLPTR};         // area view
};
#endif // ACTIONAREALIST_H
