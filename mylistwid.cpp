#include "mylistwid.h"

myListWid::myListWid(QWidget *parent) :
    QListWidget(parent)
{
}


void myListWid::mousePressEvent(QMouseEvent *event)
{
    this->x1=event->x();
    this->y1=event->y();
    emit Mouse_Pressed();
}

void myListWid::mouseReleaseEvent(QMouseEvent *e)
{
    this->x2=e->x();
    this->y2=e->y();
    emit Mouse_Released();
}
