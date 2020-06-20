//
// Created by viking on 9/9/18.
//
#include "sqlsetup.h"
#include "Paper_Model.h"
#include "constants.h"
//#include "sqlForeignKeyDelegate.h"
using namespace std;
Paper_Model::Paper_Model(const QSqlDatabase &db, QObject *parent,const QString tablename,const QString querynow ):QSqlTableModel(parent,db),table_name(tablename)
{
    setTable(QString(table_name));
    setEditStrategy(QSqlTableModel::OnManualSubmit);        // Do not save table changes to database until save request is issued
    //undoredo_dirty=false;    // this will go true if the user performs an undo or redo operation to indicate that the row edit was NOT due to a regular user edit but rather an undo or redo operation on the row
    if(querynow.length()>0)         // if querynow query is provided, then query right at instantiation using the query in querynow
    {
        qDebug()<<"from line 14 in Paper_Model.cpp query error"<<query(querynow);
    }
}


QString Paper_Model::getColumnName(const int col)     // find column name from column number
{
    return record().fieldName(col);
}



// query database model to populate this model from the database
QSqlError Paper_Model::query(const QString filter)
{
    setFilter(filter);
    select();
    auto sqlerror=lastError();
    // if this is the foreign area table model (one field only) then add in a null if one does not already exist
        if(!record(0).field(0).value().isNull() && table_name=="area")            // give user option of inserting null
        {
            auto fieldname = record(0).fieldName(0);
            auto nullrecord = QSqlRecord();
            nullrecord.setValue(fieldname,"");
            insertRecord(0,nullrecord);
        }
    if(rowCount()<0)
    {
        cout << "ERROR! from Paper_Model.cpp: query rowcount < 0 " << endl;
        exit(1);
    }
    auto rowcount=unsigned(rowCount());
    qDebug()<<"from line 50 of Paper_Model.cpp rowcount = "<<rowcount<<" filter = "<<filter<<" Error "<<sqlerror;
    if(sqlerror.type() == QSqlError::ConnectionError)
    {
        cout<<"from Paper_Model.cpp line 53 database connection ERROR on Paper Model table name " << this->tableName().toStdString()<<endl;
    }
    else if (sqlerror.type() == QSqlError::StatementError)
    {
        cout<<"from Paper_Model.cpp line 41 sql statement ERROR on Paper Model table name " << this->tableName().toStdString()<<endl;
    }
    else if (sqlerror.type() == QSqlError::TransactionError)
    {
        cout<<"from Paper_Model.cpp line 45 sql transaction ERROR on Paper Model table name " << this->tableName().toStdString()<<endl;
    }
    else if (sqlerror.type() == QSqlError::UnknownError)
    {
        cout<<"from Paper_Model.cpp line 49 sql transaction ERROR on Paper Model table name " << this->tableName().toStdString()<<endl;
    }
    if(sqlerror.type()==QSqlError::NoError && (record_redo.size()!=rowcount || record_undo.size()!=rowcount) )     // resize undo/redo stack number of rows
    {
        int numberofrows=clearstacks();                              // clear undo, redo stacks and reset original state of table records as populated from the query
        cout<<"the number of rows from query in Paper_Model.cpp line 86 ="<<numberofrows;
    }
    else
    {
        cout<<"from line 74 in Paper_Model.cpp record_redo.size() =  "<<record_redo.size()<<" record_undo.size() = "<<record_undo.size()<<endl;
    }
    return sqlerror ;   // return errors if there are errors in the query
}







// sets background color in this model's view and performs standard editing behavior
// override allows table to show which rows have been modified by the user but not yet committed to the database
QVariant Paper_Model::data(const QModelIndex &index, int role) const
{
    if(index.isValid() )
    {
        if(role==Qt::BackgroundColorRole && (tableName()=="papers" || tableName()=="area"))          // color cells only if this is a table of returned values from the papers or area columns
        {
            if(originalrecords.size()<=unsigned(index.row()))         // catch programmer screw-ups
            {
                std::cout<<"INTERNAL ERROR! from line 93 in Paper_Model.cpp, index exceeds size of originalrecords deque"<<endl;
                std::cout<< "size of originalrecords = "<<originalrecords.size()<<" index.row() = "<<index.row()<<endl;
                exit(1);
            }
            if(!comparerecords(record(index.row()),originalrecords.at(unsigned(index.row()))))      // color cells yellow if and only if the row is different than that from the last query i.e. if it's been modified
                return QVariant(QColor(Qt::yellow));
//            else if(hoverrow==index.row())
//            {
//                return QVariant(QColor(Qt::green));
//            }
            else return QVariant(QColor(Qt::white));
        }
        else return QSqlTableModel::data(index,role);           // default standard behavior to update this model's view
   }
return QVariant();
}



// override to set this model's data from the view. Allows user edits to be transferred from the view to this model
// catch and distinguish user-applied edit events from internal edit events
// note that setData() is called for each and every column of the selected record when the record is loaded from the redo stack, undo stack, or query
// override also allows table to show which rows have been modified by the user but not yet committed to the database
bool Paper_Model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // first determine if this edit is due to user or due to undo-redo. We want to push the edit on the undo-redo stack ONLY if it's due to a direct user edit
    if(index.isValid() && role==useredit)       // if and only if the user edited this table directly (role==useredit), then we should push a record onto the undo stack
    {
        auto rowu=unsigned(index.row());
        //qDebug()<<"from line 120 in Paper_Model.cpp setting data, index.row() = "<<index.row();
        record_undo.at(rowu).push_back(record(index.row()));   // push selected current record onto the selected record's undo stack just prior to editing to allow for undo
        emit(dataChanged(index,index));
        return QSqlTableModel::setData(index,value,Qt::EditRole);                       // perform default editing behavior after pushing record to undo stack to allow undos
    }
    else if(index.isValid() && role==Qt::BackgroundColorRole)           // used ONLY to force refresh of cell background colors. The parameter, value, is set to 0 and not used in this call
    {
        QModelIndex indexstart = this->index(0,0);                      // set index to update all cells in table
        QModelIndex indexstop = this->index(rowCount()-1,columnCount()-1); // set index to update all cells in table
        emit(dataChanged(indexstart,indexstop));
        return true;
    }
    else
    {
        emit(dataChanged(index,index));
        return QSqlTableModel::setData(index,value,role);                       // perform default editing behavior
    }
}






// undo/redo stack for a row in the table
// modifies the undo/redo stack member variables: record_undo and record_redo
// for example: record_undo[i] is the undo stack for the row i and is a stack of records
QSqlRecord Paper_Model::undo_redo(int undoredo, const int &row) // row is the row of the model which is being changed.
{
    qDebug()<<"from line 152 of Paper_Model.cpp row="<<row<<"undoredo ="<<undoredo;
    if(row<rowCount() && row>=0)            // is this a valid row?
    {
        bool success=false;
        auto rowu=unsigned(row);                                          // copy current row index to unsigned so we can use it, without compiler warning, as an index to an STL container
        if(!record_undo.at(rowu).empty() && undoredo==undo)          // perform undo operation if undo operation exists on undo stack
        {
            record_redo.at(rowu).push_back(record(row));                           // add what was current record to top of redo stack to allow redo
            success=setRecord(row,record_undo.at(rowu).back());                 // take top of undo stack and move it to the current record
            cout<<"from Paper_Model.cpp undo_redo undo row "<< rowu << endl;
            if(!success)
            {
                cout<<"ERROR in Paper_Model.cpp undo_redo, Could not undo "<<endl;
                exit(1);
            }
            record_undo.at(rowu).pop_back();                                         // remove record from top of undo stack because this record was sent to the current record
        }
        else if(!record_redo.at(rowu).empty() && undoredo==redo) // redo operation
        {
            record_undo.at(rowu).push_back(record(row));                           // add what was current record to top of undo stack to allow redo
            //qDebug()<<"from Paper_Model.cpp line 133 size of record_redo.at(row) "<<record_redo.at(rowu).size();
            //qDebug()<<"from Paper_Model.cpp line 134 record_redo.at(row).back() "<<record_redo.at(rowu).back();
            success=setRecord(row,record_redo.at(rowu).back());
            if(!success)
            {
                cout<<"ERROR in Paper_Model.cpp undo_redo, Could not redo "<<endl;
                exit(1);
            }
            record_redo.at(rowu).pop_back();                                         // remove record from back of stack for redo
        }
   // now update table colors
    }
    else
    {
        cout<<"ERROR in Paper_Model.cpp invalid index"<<endl;
        exit(1);
    }
return record(row);         // return current record
}






// undo all edits to this model which have not been committed to the database
// re-populates this model from the corresponding database table
// this this method also clears out the row undo and redo stacks as well as the originalrecords deque via the clearstacks method
// this method is generally called from this model's view so that the view can unhide rows which were to be deleted
void Paper_Model::undo_all_edits()
{
    for(int ir=0;ir<rowCount();ir++)
    {
        revertAll();                    // populate this model from its database table
        clearstacks();                              // clear undo, redo stacks and reset original state of table record
    }
}

// save model to database
bool Paper_Model::save_model_to_database()
    {
    bool success = submitAll();
    clearstacks();                              // clear undo, redo stacks and reset original state of table records
    cout<<"main table Paper_Model save to database, success ="<<success<<endl;
    return success;         // true if the database save was successful
}


// compare records (i.e. compare rows)
// if all values of two records are equal, then return true, else return false
bool Paper_Model::comparerecords(const QSqlRecord record1, const QSqlRecord record2) const        // compare two records
{
    if(record1.count()!=record2.count()) return false;      // records must have same number of columns (fields) in order to be equivalent
    auto recordcount=record1.count();
    for(int ifd=0;ifd<recordcount;ifd++) if(record1.value(ifd)!=record2.value(ifd)) return false;   // each field of both records must hold the same data for the records to be equivalent
    return true;
}


// delete row
// The actual deletion of the table row occurs when this model is saved to the database - the row is merely hidden, via this model's view, until this model is saved
// the model's view should also hide the row to be deleted to avoid user confusion
// deleterow is called from the view's actionslot_rowdelete() slot method which also hides the row which is to be deleted on the next database save
// originalrecords deque members are NOT deleted here because this leads to errors due to the model's rows still existing in the data() method
void Paper_Model::deleterow(int row)
{
    if(!removeRows(row,1))                  // row is actually removed ONLY upon submission to database. However we immediately hide the row and remove its redo and undo stacks
    {
        cout<<"ERROR! could not remove rows"<<endl;
    }
    auto rowu=unsigned(row);
    record_redo.erase(record_redo.begin()+rowu);    // remove corresponding row member from redo stack
    record_undo.erase(record_undo.begin()+rowu);    // remove corresponding row member from undo stack
    //cout<<"from line 246 in Paper_Model.cpp originalrecords size = "<<originalrecords.size()<<endl;
    //originalrecords.erase(originalrecords.begin()+rowu); // remove corresponding row member from originalrecords deque
    //cout<<"from line 248 in Paper_Model.cpp originalrecords size = "<<originalrecords.size()<<endl;
}



// insert empty row
void Paper_Model::insertrow()
{
    //cout<<"from Paper_Model::insertrow() before insert row count "<<rowCount()<<endl;
    bool insertOK = insertRow(rowCount());
    //cout<<"from Paper_Model::insertrow() after insert row count "<<rowCount()<<endl;
    if(!insertOK)                  // row is actually removed ONLY upon submission to database. However we immediately hide the row and remove its redo and undo stacks
    {
        cout<<"ERROR! could not insert new empty row "<<endl;
        exit(1);
    }
    record_redo.resize(rowCount());    // allocate rows
    record_undo.resize(rowCount());    // allocate rows
    originalrecords.resize(rowCount()); // original records, add empty row to originalrecords deque
}





// get data from selected cell
QVariant Paper_Model::getcelldata(const int row, const int column)
{
    QVariant dataq;
    dataq=index(row,column).data();
    return(dataq);
}
QVariant Paper_Model::getcelldata(const QModelIndex setindex)
{
    QVariant dataq;
    dataq=data(setindex,Qt::DisplayRole);
    return(dataq);
}


// clear out undo and redo stack. Also clear out and reset the originalrecords and reset it to the results as populated from the database table
int Paper_Model::clearstacks()
{
    // both the redo and undo stack size need to be the number of rows in this model -> i.e. a stack of records for each row
    record_redo.clear();             // remove all elements in records redo stack prior to new query
    record_undo.clear();            // remove all elements in records undo stack prior to new query
    record_redo.resize(rowCount());    // allocate rows
    record_undo.resize(rowCount());    // allocate rows
    originalrecords.clear();
    // save table state to allow reversal of further user edits which occur after this save to the database
    for(int i=0;i<rowCount();i++)
    {
        originalrecords.push_back(record(i));         // save query result row to original records so user can go back to original if desired
    }
    return rowCount();
}
