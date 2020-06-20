#ifndef SQLTABLEHEADER_H
#define SQLTABLEHEADER_H
#include <constants.h>
#include <sqlsetup.h>
#include<Paper_Model.h>
#include<ActionTable.h>
class sqlTableHHeader: public QHeaderView
{
    Q_OBJECT
public:
    sqlTableHHeader(ActionTable * table = Q_NULLPTR,Paper_Model * papermodel=Q_NULLPTR, Paper_Model * areamodel = Q_NULLPTR);
    virtual ~sqlTableHHeader() override;
    void mousePressEvent(QMouseEvent *) override;
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
private:
    Paper_Model * papermodel{nullptr};      // alias for paper table model
    Paper_Model * areamodel {nullptr};      // alias for area table model
    ActionTable * tabledialog{nullptr};     // alias for parent widget
    int selectedindex=-1;
    //void click
};

#endif // SQLTABLEHEADER_H
