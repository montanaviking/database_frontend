//
// Created by viking on 9/9/18.
//
// table model
#ifndef SQL_SQLTABLEMODEL_H
#define SQL_SQLTABLEMODEL_H
#include <sqlsetup.h>
using namespace std;
class Paper_Model: public QSqlTableModel {
        Q_OBJECT
public:
    explicit Paper_Model(const QSqlDatabase &db, QObject* parent=Q_NULLPTR,const QString tablename="",const QString querynow="");
    QString getColumnName(const int col);
    QString const table_name;                       // name of underlying database table operated on by this model, initiated on instantiation
    QSqlError query(const QString filter=QString("1"));     // performs query to the underlying database table whose database name is contained in the member, table_name
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole) override;
    QVariant data(const QModelIndex &index, int role) const override;
    QSqlRecord undo_redo(int undoredo,const int & row);      //undo model edit
    QVariant getcelldata(const int row,const int column);               // return data from selected cell
    QVariant getcelldata(const QModelIndex setindex);               // return data from selected cell
    //int hoverrow{-1};

public slots:
    void undo_all_edits();                  // undo all pending edits.
    bool save_model_to_database();           // save model data to database
    void deleterow(int row);                 // also deletes row from undo and redo stacks, this action is reversible ONLY by repopulating this model from the database which resets the
    void insertrow();                       // insert empty row. Also add empty rows to undo and redo stacks and originalrecords vector

private:
    deque<list<QSqlRecord>> record_undo;     // deque of lists which holds copies stack of record under edit, each member of the deque represents a row (i.e. record) in the query resulted table and each list within the deque are the records (rows) to be undone
    deque<list<QSqlRecord>> record_redo;     // deque of lists which holds copies stack of record under edit, each member of the deque represents a row (i.e. record) in the query resulted table and each list within the deque are the records (rows) to be redone
    deque<QSqlRecord> originalrecords;            // record as returned from query, prior to edit, one record per row in the query resulted table
    bool comparerecords(const QSqlRecord record1, const QSqlRecord record2) const;        // compare two records
    int clearstacks();                         // clears record_undo, record_redo, and originalrecords and sets them up.
};

#endif //SQL_SQLTABLEMODEL_H
