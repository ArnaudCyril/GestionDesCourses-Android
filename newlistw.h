#ifndef NEWLISTW_H
#define NEWLISTW_H

#include <QListWidget>
#include <QMouseEvent>
#include <QEvent>
class NewListW : public QListWidget
{
    Q_OBJECT
public:
    explicit NewListW(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *e);
    int x1,x2;
signals:
    void Mouse_Pressed();
    void Mouse_Released();
public slots:

};

#endif // NEWLISTW_H
