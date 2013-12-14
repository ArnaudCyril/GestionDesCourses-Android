#ifndef MYLISTWID_H
#define MYLISTWID_H

#include <QListWidget>
#include <QMouseEvent>
#include <QEvent>

class myListWid : public QListWidget
{
    Q_OBJECT
public:
    explicit myListWid(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *e);
    int x1,x2,y1,y2;
signals:

    void Mouse_Pressed();
    void Mouse_Released();
public slots:

};

#endif // MYLISTWID_H
