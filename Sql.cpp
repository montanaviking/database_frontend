//
// Created by viking on 7/4/18.
//

#include <QtWidgets/QMessageBox>
#include <QApplication>
#include "Sql.h"

Sql::Sql(const QString hname, const QString dbname, const QString user, const QString pass ):
    hostname(hname),databasename(dbname),username(user),password(pass)

{
    connect();
}

Sql Sql::select(const QString &columns, const QString &table) {
        sqlquery+=QString("SELECT ")+columns+" FROM "+table;
        return *this;
    };

Sql Sql::where(const QString & conditions) {
    //check the Boolean expression statement for consistency
    auto const leftparacount=conditions.count("(");
    auto const rightparacount=conditions.count(")");
    if (leftparacount!=rightparacount) std::cout<<"ERROR! parethesis don't match";
    sqlquery+=QString(" WHERE ")+conditions;
    return *this;
    };
void Sql::connect()
    {
        db=QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName(hostname);
        db.setDatabaseName(databasename);
        db.setUserName(username);
        db.setPassword(password);
        bool ok=db.open();
        //
        if(!ok) {
            QMessageBox mess;
            mess.setText("ERROR,CANNOT open database");
            mess.exec();
            exit(1);
            }
    }
/*
Sql Sql::lpar()
    {
        sqlquery=QString(" ( ")+sqlquery;
        return *this;
    }

Sql Sql::rpar()
    {
        sqlquery=sqlquery+QString(" ) ");
        return *this;
    }

Sql Sql::and_(const QString & condition)
    {
        sqlquery = sqlquery + QString(" AND ") + condition;
        return *this;
    }

Sql Sql::or_(const QString & condition)
{
    sqlquery = sqlquery + QString(" OR ") + condition;
    return *this;
}

Sql Sql::not_(const QString & condition)
{
    sqlquery = sqlquery + QString(" NOT ") + condition;
    return *this;
}
*/
const QString Sql::getsql() {return sqlquery;}
QSqlQuery Sql::qry(QString qq) {
    QSqlQuery q;
    if (!q.exec(qq)) {
        QMessageBox mess;
        mess.setText(q.lastError().text());
        mess.exec();
        exit(1);
    }
    return(q);
}

