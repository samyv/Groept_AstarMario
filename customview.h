#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H

#include <QGraphicsView>
#include <QtWidgets>
#include <QWidget>

class Customview: public QGraphicsView
{
    Q_OBJECT
public:
    explicit Customview(QWidget* parent = nullptr);
    int _numScheduledScalings = 0;
    void keyPressEvent(QKeyEvent *event);
signals:
    void marioMoved(int,int);
protected:
    virtual void wheelEvent ( QWheelEvent * event );
public slots:
    void scalingTime(qreal x);
    void animFinished();
};

#endif // CUSTOMVIEW_H
