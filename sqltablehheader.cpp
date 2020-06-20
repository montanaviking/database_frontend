#include "sqlsetup.h"
#include "sqltablehheader.h"
#include "areaview.h"

//This is the table header for sqlTableView
sqlTableHHeader::sqlTableHHeader(ActionTable* tabledialog, Paper_Model *papermodel,Paper_Model *areamodel): QHeaderView(Qt::Horizontal,dynamic_cast<QWidget *>(tabledialog)), papermodel(papermodel),areamodel(areamodel),tabledialog(tabledialog)
{
   if(papermodel==Q_NULLPTR)
   {
       std::cout<<"ERROR from sqltableheader.cpp: model is NULL"<<std::endl;
       std::exit(1);
   }
   setModel(dynamic_cast<QAbstractItemModel *>(papermodel));         // this model is from the "papers" table
   setHighlightSections(true);
}
sqlTableHHeader::~sqlTableHHeader(){}
void sqlTableHHeader::mousePressEvent(QMouseEvent * me)
{
    auto index = logicalIndexAt(me->pos());
    if(me->button()==Qt::RightButton && (index==AREACOL || index==AREA2COL))    // AREACOL and AREA2COL are the foreign key column numbers
    {
        selectedindex=index;          // get header index selected by right mouse on header
        viewport()->repaint();
        if (tabledialog!=Q_NULLPTR)
        {
           //auto listeditor = new ActionAreaList(tabledialog->db,tabledialog);
           auto listeditor = new ActionAreaList(tabledialog,areamodel);
           listeditor->show();        // show editor to change "area"
        }
        return;
    }
    else
    {
        selectedindex=-1;
        viewport()->repaint();
    }
    QHeaderView::mousePressEvent(me);                   // perform default mouse action on headers
}
void sqlTableHHeader::paintSection(QPainter* painter, const QRect& rect, int index) const
{
        if(index==selectedindex && (selectedindex==AREACOL || selectedindex==AREA2COL))
      {
             //QBrush brush= qvariant_cast<QBrush>(colIndex.data(Qt::BackgroundRole));
             auto brush = QBrush(Qt::SolidPattern);
             brush.setColor(Qt::red);
             QString text=papermodel->headerData(index,orientation(),Qt::DisplayRole).toString();
             painter->save();
             painter->setBrush(brush);
             painter->drawRect(rect);
             painter->setPen(Qt::black);
             painter->drawText(rect,Qt::AlignCenter,text);
             painter->restore();
             return;
        }
    else {QHeaderView::paintSection(painter, rect, index);}         //default painting
}
