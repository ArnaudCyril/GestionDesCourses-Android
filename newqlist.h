#ifndef NEWQLIST_H
#define NEWQLIST_H

#include <QListWidget>
#include <QEvent>
#include <QMouseEvent>
class NewQList : public QListWidget
{
    Q_OBJECT
public:
    explicit NewQList(QWidget *parent = 0);
    //void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *e);

signals:
    void MoussePress();
    void MousseReleased();
public slots:

};

#endif // NEWQLIST_H
