#ifndef CUBICBEZIERTOOLS_H
#define CUBICBEZIERTOOLS_H

#include "mathtools.h"

/**
 * This class contains mathematical methods which are only for Bezier curves.
 */
class CubicBezierTools
{
public:
    CubicBezierTools();
    /**
     * To calculate a point on a Bezier curve according to the parameter t.
     *
     * @param iT                the parameter which determines the position of the
     *                          point. The range of it lies in [0.0, 1.0]
     * @param iPosA             the start point of the Bezier curve
     * @param iControlPointA    the control point which is close to the start point
     * @param iControlPointB    the control point which is close to the end point
     * @param iPosB             the end point of the Bezier curve
     * @return a point on the current Bezier curve
     */
    static QPointF pointOnBezierCurve(qreal iT, const QPointF &iPosA, const QPointF &iControlPointA,
                                      const QPointF &iControlPointB, const QPointF &iPosB);

    /**
     * To calculate the unit tangent vector of a point on the bezier curve.
     *
     * @param iPosA             the start point of the Bezier curve
     * @param iControlPointA    the control point which is close to the start point
     * @param iControlPointB    the control point which is close to the end point
     * @param iPosB             the end point of the Bezier curve
     * @param iT                a parameter to determine the position on curve
     *                          which must lies in the range [0, 1]
     * @return the unit tangent vector of the point on the Bezier curve
     */
    static QPointF calculateUnitTangentVectorOfBezierCurve(const QPointF &iPosA, const QPointF &iControlPointA,
                                                           const QPointF &iControlPointB, const QPointF &iPosB, double iT);

    /**
     * Let P0, P1, P2, P3 be the start point, first control point, second
     * control point and the end point of the Bezier curve. For any point Q on
     * the Bezier curve, Q(t) = P0*(1-t)^3 + P1*3t(1-t)^2 + P2*3t^2(1-t) + P3*t^3
     * = (x(t), y(t)). This method is to calculate Q'(t) = (x'(t), y'(t)).
     *
     * @param iPosA             the start point of the Bezier curve
     * @param iControlPointA    the control point which is close to the start point
     * @param iControlPointB    the control point which is close to the end point
     * @param iPosB             the end point of the Bezier curve
     * @param iT                a parameter to determine the position on the
     *                          Bezier curve
     * @return Q'(t) = (x'(t), y'(t))
     */
    static QPointF calculateDerivativeOnBezierCurve(
            const QPointF &iPosA, const QPointF &iControlPointA,
            const QPointF &iControlPointB, const QPointF &iPosB, double iT);
};

#endif // CUBICBEZIERTOOLS_H
