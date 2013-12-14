#ifndef THEPAGE_H
#define THEPAGE_H

#include <QWidget>
#include <QEvent>
#include <QMouseEvent>

class ThePage : public QWidget
{
    Q_OBJECT
public:
    explicit ThePage(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    int x1,x2;

signals:
    void MoussePressed();
    void MousseReleased();
public slots:

};

#endif // THEPAGE_H
