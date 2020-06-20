// reimplementation of query table view
// Created by viking on 8/26/18.
// View for main table
#ifndef SQL_QUERYTABLEVIEW_H
#define SQL_QUERYTABLEVIEW_H

#include <sqlsetup.h>
#include "Paper_Model.h"



class SqlTableView: public QTableView
{
    Q_OBJECT

public:
    SqlTableView(QWidget *parent=Q_NULLPTR);
    void setModel(QAbstractItemModel *model) override;

signals:
    void signalhoverindexchanged(QModelIndex &);
public slots:
    void undo_all_edits();                                      // re-populates model of this view from the database table, thus removing all changes
    //bool save_model_to_database();                              // saves model of this view to the database. Returns true if successful and false if unsuccessful

private slots:
    void actionslot_rowdelete();
    void actionslot_rowinsert();
    void actionslot_openlinkedfile();
    void actionslot_setlink();
    //void actionslot_rowcopy();

private:
    void keyPressEvent(QKeyEvent *ke) override;
    void mousePressEvent(QMouseEvent *me) override;
    void mouseMoveEvent(QMouseEvent *me) override;     // save edits on loss-of-focus, Note that this requires setMouseTracking(true)
    Paper_Model *pmodel{Q_NULLPTR};                    // alias of model
};

#endif //SQL_QUERYTABLEVIEW_H
