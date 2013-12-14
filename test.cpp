#include "test.h"
#include <QDebug>
#include <QMimeData>
test::test(QWidget *parent) :
    QListWidget(parent)
{
    setDragDropMode(QAbstractItemView::InternalMove);
    setDragDropOverwriteMode(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setDefaultDropAction(Qt::MoveAction);
    this->setAcceptDrops(true);

}

void test::dragEnterEvent(QDragEnterEvent *event)
{
    emit DragStart();

    qDebug()<<"1";
}

void test::dropEvent(QDropEvent *event)
{
    emit DropOk();qDebug()<<"2";
}

void test::dragLeaveEvent(QDragLeaveEvent *e)
{
    emit DragLeave();qDebug()<<"3";
}

void test::dragMoveEvent(QDragMoveEvent *e)
{
    emit DragMove();qDebug()<<"4";
}

void test::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

          QDrag *drag = new QDrag(this);
          QMimeData *mimeData = new QMimeData;

          mimeData->setText("Test");
          drag->setMimeData(mimeData);


          Qt::DropAction dropAction = drag->exec();
      }
}
