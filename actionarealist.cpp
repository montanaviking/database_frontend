#include <actionarealist.h>
#include <sqllistdelegate.h>

// widget involked by user to edit area table
ActionAreaList::ActionAreaList(QDialog *parent, Paper_Model *areamodel):QDialog(parent),Ui_area_Editor(),areamodel(areamodel)
{
    setupUi(this);
    //model = new Paper_Model(dB,this,"area");
    sqlAreaView = new areaView(this);
    arealistbox->addWidget(sqlAreaView);
    auto sqlerror=query_area_table();                      // get all entries from area table model query
    sqlAreaView->setModel(areamodel);
    auto *sqlarelistdelegate = new sqlListDelegate(sqlAreaView);        //delegate for area view where area view is a foreign key of the main table
    sqlAreaView->setItemDelegate(sqlarelistdelegate);
    setAttribute(Qt::WA_DeleteOnClose,true);
    // buttons
    connect(undo_all_edits_button,SIGNAL(clicked()),areamodel,SLOT(undo_all_edits()));      // button to undo all edits. Effective ONLY prior to sending to database via save button
    //connect(save_button,SIGNAL(clicked()),model,SLOT(save_model_to_database()));        // save model to database. We also need to display a table of all the rows of "papers" database table, which will need to be modified to reflect
                                                                                        // the changes to the "area" table in the database. This is necessary to maintain database integrity.
}

QSqlError ActionAreaList::query_area_table(QString filter) const        // query of database
{
    auto sqlerror=areamodel->query(filter);
    return sqlerror;
}
