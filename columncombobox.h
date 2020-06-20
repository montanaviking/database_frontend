#ifndef COLUMNCOMBOBOX_H
#define COLUMNCOMBOBOX_H
#include "sqlsetup.h"

class ColumnComboBox: public QComboBox
{
    Q_OBJECT
public:
   explicit ColumnComboBox(QWidget *parent = Q_NULLPTR);
protected:
   void mousePressEvent(QMouseEvent *e) override;
};

#endif // COLUMNCOMBOBOX_H
