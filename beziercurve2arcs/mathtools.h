#ifndef MATHTOOLS_H
#define MATHTOOLS_H

#include <qmath.h>
#include <QPointF>
#include <QLineF>

/* a constant which represents a quite small number */
const double EPSILON = 1e-9;

struct Arc {
    QPointF center;
    qreal radius = 0;
    /* the start angle of the arc which lies in the range [-pi, pi] */
    qreal startAngle = 0;
    /* the end angle of the arc which lies in the range
       [{@code startAngle} - pi, {@code startAngle} + pi] */
    qreal endAngle = 0;
    /* a variable to determine the direction of the arc.
       True for clockwise and false for anti-clockwise */
    bool clockwiseFlag = true;
    Arc(const QPointF &iCenter = QPointF(), qreal iRadius = 0, qreal iStartAngle = 0,
        qreal iEndAngle = 0, bool iClockwiseFlag = true);
};
/**
 * This class contains different mathematical tools.
 */
class MathTools
{
public:
    MathTools();

    /**
     * To calculate the intersection point of two line.
     *
     * @param iStartPointA the start point of the first line
     * @param iEndPointA   the end point of the first line
     * @param iStartPointB the start point of the second line
     * @param iEndPointB   the end point of the second line
     * @return the coordinate of the intersection point
     */
    static QPointF calculateIntersectionOfTwoLine(
            QPointF iStartPointA, QPointF iEndPointA,
            QPointF iStartPointB, QPointF iEndPointB);

    /**
     * To calculate the cross-product of vector p0p1 and vector p0p2.
     *
     * @param p0 the first point
     * @param p1 the second point
     * @param p2 the third point
     * @return p0p1 · p0p2
     */
    static double crossProductOfThreePoints(
            QPointF p0, QPointF p1, QPointF p2);

    /**
     * To calculate a point on the given segment according to the λ parameter
     *
     * @param lambda the interval proportion
     * @param A      start point of the line
     * @param B      end point of the line
     * @return a point on the segment AB according to {@code lambda}
     */
    static QPointF calculateIntervalPoint(
            double lambda, QPointF A, QPointF B);

    /**
     * To calculate the Euclidean distance between two points
     *
     * @param A the first point
     * @param B the second point
     * @return the Euclidean distance between two points
     */
    static double euclideanDistance(QPointF A, QPointF B);

    /**
     * To find the incenter of a triangle.
     *
     * @param p1 the first vertex of the triangle
     * @param p2 the second vertex of the triangle
     * @param p3 the third vertex of the triangle
     * @return the incenter of a triangle
     */
    static QPointF findInCenterPointOfTriangle(
            QPointF p1, QPointF p2, QPointF p3);

    /**
     * To solve the equation:
     * <p>
     * a11*x + a12*y = b1
     * a21*x + a22*y = b2.
     *
     * @param a11 coefficient 1
     * @param a12 coefficient 2
     * @param b1  parameter1
     * @param a21 coefficient 3
     * @param a22 coefficient 4
     * @param b2  parameter2
     * @return (x, y) in {@code QPointF} type
     */
    static QPointF solveLinearEquationsOfTwoUnknownVariables(
            double a11, double a12, double b1,
            double a21, double a22, double b2);

    /**
     * To calculate the result of
     * <p>
     * |a11, a12|
     * |a21, a22|.
     *
     * @param a11 the left-top element
     * @param a12 the right-top element
     * @param a21 the left-bottom element
     * @param a22 the right-bottom element
     * @return the result of
     * |a11, a12|
     * |a21, a22|.
     */
    static double calculateSecondOrderDeterminant(
            double a11, double a12, double a21, double a22);

    /**
     * To calculate the unit tangent vector of a point on circle.
     *
     * @param center        the center of the circle
     * @param pointOnCircle a point on the circle
     * @return the unit tangent vector of the point on the circle
     */
    static QPointF calculateUnitTangentVectorOfCircle(
            QPointF center, QPointF pointOnCircle);

    /**
     * To calculate the dot-product: p1·p2.
     *
     * @param p1 the first vector
     * @param p2 the second vector
     * @return the result of the dot-product: p1·p2
     */
    static double dotProductOfTwoPoints(QPointF p1, QPointF p2);

    /**
     * To calculate the angle from a point to the center of the circle and the
     * x axis. The return value lies in the range [-pi, pi].
     *
     * @param center the center of the circle
     * @param point  a point on the circle
     * @return Let {@code center} be point O, and let {@code point} be point A
     * and let a point on the positive x-axis be point X, then the calculated
     * angle is <AOX. The return value lies in the range [-pi, pi].
     */
    static double calculateAngleRelativeToCircleCenter(
            QPointF center, QPointF point);
};

#endif // MATHTOOLS_H
