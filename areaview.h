#ifndef AREAVIEW_H
#define AREAVIEW_H

#include <sqlsetup.h>
#include <Paper_Model.h>
// foreign key view
class areaView:public QListView
{
    Q_OBJECT

public:
    areaView(QWidget * parent=Q_NULLPTR);
    void setModel(QAbstractItemModel *model) override;

//signals:
    //void signalhoverindexchanged(QModelIndex &);

private slots:
    void actionslot_rowdelete();
    void actionslot_rowinsert();

private:
    void keyPressEvent(QKeyEvent *ke) override;
    void mousePressEvent(QMouseEvent *me) override;
    void mouseMoveEvent(QMouseEvent *me) override;      // requires setMouseTracking(true)

    Paper_Model *pmodel{Q_NULLPTR};                    // alias of paper model
};

#endif // AREAVIEW_H
