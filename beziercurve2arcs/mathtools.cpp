#include "mathtools.h"

MathTools::MathTools()
{

}

QPointF MathTools::calculateIntersectionOfTwoLine(QPointF iStartPointA, QPointF iEndPointA, QPointF iStartPointB, QPointF iEndPointB) {
    QPointF intersectPos;
    QLineF::IntersectType type = QLineF(iStartPointA, iEndPointA).intersect(QLineF(iStartPointB, iEndPointB), &intersectPos);
    if (type != QLineF::NoIntersection) {
        return intersectPos;
    }
    return QPointF();
}

double MathTools::crossProductOfThreePoints(QPointF p0, QPointF p1, QPointF p2) {

    double x1 = p1.x() - p0.x();
    double y1 = p1.y() - p0.y();
    double x2 = p2.x() - p0.x();
    double y2 = p2.y() - p0.y();

    return x1 * y2 - x2 * y1;
}

QPointF MathTools::calculateIntervalPoint(double lambda, QPointF A, QPointF B) {

    return QPointF(
                A.x() + lambda * (B.x() - A.x()),
                A.y() + lambda * (B.y() - A.y())
                );
}

double MathTools::euclideanDistance(QPointF A, QPointF B) {

    return sqrt(
                (A.x() - B.x()) * (A.x() - B.x())
                + (A.y() - B.y()) * (A.y() - B.y()));
}

QPointF MathTools::findInCenterPointOfTriangle(QPointF p1, QPointF p2, QPointF p3) {

    double a = euclideanDistance(p2, p3);
    double b = euclideanDistance(p1, p3);
    double c = euclideanDistance(p1, p2);

    double x1 = p1.x();
    double y1 = p1.y();

    double x2 = p2.x();
    double y2 = p2.y();

    double x3 = p3.x();
    double y3 = p3.y();

    return QPointF((a * x1 + b * x2 + c * x3) / (a + b + c),
                       (a * y1 + b * y2 + c * y3) / (a + b + c));
}

QPointF MathTools::solveLinearEquationsOfTwoUnknownVariables(double a11, double a12, double b1, double a21, double a22, double b2) {

    double x =
            calculateSecondOrderDeterminant(b1, a12, b2, a22) /
            calculateSecondOrderDeterminant(a11, a12, a21, a22);

    double y =
            calculateSecondOrderDeterminant(a11, b1, a21, b2) /
            calculateSecondOrderDeterminant(a11, a12, a21, a22);

    return QPointF(x, y);
}

double MathTools::calculateSecondOrderDeterminant(double a11, double a12, double a21, double a22) {

    return a11 * a22 - a12 * a21;
}

QPointF MathTools::calculateUnitTangentVectorOfCircle(QPointF center, QPointF pointOnCircle) {

    double x1 = center.x();
    double y1 = center.y();
    double x2 = pointOnCircle.x();
    double y2 = pointOnCircle.y();

    if (abs(x1 - x2) <= EPSILON) {
        if (y1 < y2) {
            return QPointF(1.0, 0.0);
        } else {
            return QPointF(-1.0, 0.0);
        }
    }

    double radius = euclideanDistance(center, pointOnCircle);

    double deltaX = x2 - x1;
    double deltaY = y2 - y1;

    return QPointF(deltaY / radius, 0.0 - deltaX / radius);
}

double MathTools::dotProductOfTwoPoints(QPointF p1, QPointF p2) {

    return p1.x() * p2.x() + p1.y() * p2.y();
}

double MathTools::calculateAngleRelativeToCircleCenter(QPointF center, QPointF point) {

    double deltaX = point.x() - center.x();
    double deltaY = point.y() - center.y();

    return atan2(deltaY, deltaX);
}

Arc::Arc(const QPointF &iCenter, qreal iRadius, qreal iStartAngle, qreal iEndAngle, bool iClockwiseFlag)
{

    center = iCenter;
    radius = iRadius;
    startAngle = iStartAngle;
    endAngle = iEndAngle;
    clockwiseFlag = iClockwiseFlag;
}
