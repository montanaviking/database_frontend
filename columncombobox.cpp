#include "columncombobox.h"

ColumnComboBox::ColumnComboBox(QWidget * parent):QComboBox (parent){}

void ColumnComboBox::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::RightButton)
    {
        std::cout<<"rightbutton from ColumnComboBox"<<std::endl;

    }
    else{
        QComboBox::mousePressEvent(e);          // standard mouse handleing as default
    }
}
