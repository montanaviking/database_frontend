#include <iostream>
#include <sqlsetup.h>
#include <QDebug>
#include <Sql.h>
#include <QMessageBox>
#include "ActionTable.h"
#include "Paper_Model.h"

int main(int argc, char ** argv) {
    QApplication app(argc,argv);
   /* auto sq = Sql("localhost","Probeplantest","montanaviking","nova");
    auto table=QString("Device");
    auto column=QString("|Idmax|");

    const auto c = "|Idmax|>100,on-off>20,fmax<30";
    sq.select(column,table);
    //const auto d=and_(c);
    //auto d= QString("test");
    sq.where(and_(c));
    qDebug()<<sq.getsql();
    auto q=sq.qry("SELECT base_name FROM Device_Template WHERE relative_x=125");
    auto rec=q.record();
    while(q.next())
        qDebug()<<q.value(rec.indexOf("base_name")).toString(); */
    //sq.connect("localhost","Probeplantest","montanaviking","nova");
    /*const auto c = QString("|Idmax|>100,on-off>20,fmax<30");
    auto ql=and_(c);
    qDebug()<<ql; */
//    int argc;
//    char ** argv;

//    QMessageBox msg;
//    msg.setText("ERROR,CANNOT open database");
//    msg.exec();
//    QMessageBox::critical(nullptr,"test","test2");
//    return 0;


    ActionTable tbl(QString("librarytest"));
    qDebug() << tbl.query_papers_table(QString("area LIKE '%RF noise%'")).databaseText();
    tbl.show();
    return app.exec();
}
