//
// Created by viking on 7/4/18.
//
#include <sqlsetup.h>
#ifndef TESTSTRING_SQL_H
#define TESTSTRING_SQL_H


class Sql {
public:
    //QString execute(QString) const;
    Sql(QString, QString, QString, QString);
    virtual ~Sql(){;}

    Sql select(const QString &,const QString &);    // Select(column, from table)
    Sql where(const QString &);
/*    Sql lpar();        // left parenthesis
    Sql rpar();        // right parenthesis
    Sql and_(const QString &);
    Sql or_(const QString &);
    Sql not_(const QString &); */
/*    Sql orderby(const QString &);
    Sql fetchone();
    Sql fetchall(); */
    const QString getsql();
    QSqlQuery qry(QString);
    QSqlDatabase db;
    const QString hostname;
    const QString databasename;
    const QString username;
    const QString password;

private:
    QString sqlquery;
    void connect();
    //QSqlQuery q;

};

const QString and_(const QString &);
const QString or_(const QString &);
const QString not_(const QString &);
#endif //TESTSTRING_SQL_H
