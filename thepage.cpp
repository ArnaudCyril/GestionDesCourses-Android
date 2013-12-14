#include "thepage.h"

ThePage::ThePage(QWidget *parent) :
    QWidget(parent)
{
}

void ThePage::mousePressEvent(QMouseEvent *e)
{
 this->x1=e->x();
    emit MoussePressed();
}

void ThePage::mouseReleaseEvent(QMouseEvent *e)
{
    this->x2=e->x();
    emit MousseReleased();
}
