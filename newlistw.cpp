#include "newlistw.h"

NewListW::NewListW(QWidget *parent) :
    QListWidget(parent)
{
}

void NewListW::mousePressEvent(QMouseEvent *event)
{
    this->x1=event->x();
    emit Mouse_Pressed();
}

void NewListW::mouseReleaseEvent(QMouseEvent *e)
{
    this->x2=e->x();
    emit Mouse_Released();
}
