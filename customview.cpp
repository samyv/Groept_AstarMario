#include "customview.h"

Customview::Customview(QWidget * parent): QGraphicsView (parent)
{

}

void Customview::wheelEvent (QWheelEvent * event )
{
 int numDegrees = event->delta() / 8;
 int numSteps = numDegrees / 15; // see QWheelEvent documentation
 _numScheduledScalings += numSteps;
 if (_numScheduledScalings * numSteps < 0) // if user moved the wheel in another direction, we reset previously scheduled scalings
 _numScheduledScalings = numSteps;

 QTimeLine *anim = new QTimeLine(350, this);
 anim->setUpdateInterval(20);

 connect(anim, SIGNAL (valueChanged(qreal)), this, SLOT (scalingTime(qreal)));
 connect(anim, SIGNAL (finished()), this, SLOT (animFinished()));
 anim->start();
    /*setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scalefactor = 1.5;
    if(event->delta() > 0){
        scale(scalefactor, scalefactor);
    } else {
        scale(1/scalefactor, 1/scalefactor);
    }*/
    //event->accept();
}

void Customview::scalingTime(qreal x)
{
 qreal factor = 1.0+ qreal(_numScheduledScalings) / 300.0;
 scale(factor, factor);
}

void Customview::animFinished()
{
 if (_numScheduledScalings > 0)
 _numScheduledScalings--;
 else
 _numScheduledScalings++;
 sender()->~QObject();
}
