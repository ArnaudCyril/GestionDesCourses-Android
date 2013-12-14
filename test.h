#ifndef TEST_H
#define TEST_H

#include <QListWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDrag>
#include <QMouseEvent>
#include <QMoveEvent>

class test : public QListWidget
{
    Q_OBJECT
public:
    explicit test(QWidget *parent = 0);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void mousePressEvent(QMouseEvent *event);

signals:
    void DragStart();
    void DropOk();
    void DragLeave();
    void DragMove();


public slots:

};

#endif // TEST_H
