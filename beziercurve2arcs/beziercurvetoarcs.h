#ifndef BEZIERCURVETOARCS_H
#define BEZIERCURVETOARCS_H

#include <QPointF>
#include <QVector>
#include "mathtools.h"

/* A constant which represents the maximum allowable error of finding t */
const double AllOWABLE_ERROR_FOR_FIND_T = 0.001;

/**
 * This class contains methods to convert a cubic Bezier curve to a series of
 * arcs.
 * <p>
 * The algorithm is from the paper "Approximation of a planar cubic Bezier
 * spiral by circular arcs. D.J. Walton, D.S. Meek, Department of Computer
 * Science, University of Manitoba, Winnipeg, Man., Canada R3T 2N2".
 * <p>
 * Note that the input cubic Bezier curve should have an angle of less than 180
 * degrees from the start point of to the end point. If not so, the result
 * might be wrong.
 */
class BezierCurveToArcs
{
public:
    BezierCurveToArcs();
    /**
     * To convert a cubic Bezier curve to a series of arcs.
     *
     * @param iPosA           the start point of the Bezier curve
     * @param iControlPointA  the first control point of the Bezier curve which
     *                        is close to iPosA
     * @param iControlPointB  the second control point of the Bezier curve which
     *                        is close to iPosB
     * @param iPosB           the end point of the Bezier curve
     */
    static QVector<Arc> convertACubicBezierCurveToArcs(const QPointF &iPosA,
                                                       const QPointF &iControlPointA,
                                                       const QPointF &iControlPointB,
                                                       const QPointF &iPosB, double iAllowableError);
    /**
     * This function is an auxiliary function for the method
     * {@code convertACubicBezierCurveToArcs}. The aim is to generate fitted
     * arcs from the original Bezier curve according to the range of t.
     *
     * @param iPosA              the start point of the Bezier curve
     * @param iControlPointA     the control point which is close to the start point
     * @param iControlPointB     the control point which is close to the end point
     * @param iPosB              the end point of the Bezier curve
     * @param iStartT            the start t value for Bezier curve. It is a
     *                           parameter to determine the start range. The range
     *                           of it should lies in [0.0, iEndT]
     * @param iEndT              the end t value for Bezier curve. It is a
     *                           parameter to determine the end range. The range
     *                           of it should lies in [iStartT, 1.0]
     */
    static QVector<Arc> convertACubicBezierCurveToArcsHelper(const QPointF &iPosA, const QPointF &iControlPointA,
                                                             const QPointF &iControlPointB, const QPointF &iPosB,
                                                             qreal iStartT, qreal iEndT, double iAllowableError);

    /**
         * To find the t value so that (Q(t) - G) · H = 0, where
         * Q(t) = P0 * (1-t)^3 + P1 * 3t(1-t)^2 + P2 * 3t^2(1-t) + P3 * t^3. (P0,
         * P1, P2, P3 are the start point, the first control point, the second
         * control point and the end point of the current Bezier curve).
         * <p>
         * Newton-Raphson method is used in this function because
         * f(t) = (Q(t) - G) · H is monotone for t which lies in the range
         * [{@code startT}, {@code endT}. In Newton-Raphson method,
         * tn+1 = tn - f(tn)/f'(tn).
         *
         * @param A              the start point of the Bezier curve
         * @param controlPointA  the first control point which is close to the
         *                       start point
         * @param controlPointB  the second control point which is close to the
         *                       end point
         * @param B              the end point of the Bezier curve
         * @param H              the unit tangent vector on point {@code G} of the
         *                       biarc
         * @param G              the intersection point of biarc
         * @param allowableError the allowable error for
         *                       f(t) <= {@code allowableError}
         * @param startT         the t parameter which determines the start position
         * @param endT           the t parameter which determines the end position
         * @return the t value which makes f(t) is near to zero
         */
        static double findTWithNewtonAndRaphsonMethod(
                QPointF A, QPointF controlPointA,
                QPointF controlPointB, QPointF B,
                QPointF H, QPointF G, double allowableError,
                double startT, double endT);

        /**
         * Let the point {@code newA} be the start point of the current Bezier
         * curve and the point {@code newB} be the end point of the current Bezier
         * curve. Let L1 be the tangent line to the Bezier curve on the point
         * {@code newA} and L2 be the tangent line to the Bezier curve on the point
         * {@code newB}. Then the point V is the intersection point of L1, L2. This
         * function is to calculate the coordinate of V.
         *
         * @param newA          the new start point of the Bezier curve
         * @param newB          the new end point of the Bezier curve
         * @param A             the original start point of the Bezier curve
         * @param controlPointA the first control point of the Bezier curve which
         *                      is close to {@code A}
         * @param controlPointB the second control point of the Bezier curve which
         *                      is close to {@code B}
         * @param B             the original end point of the Bezier curve
         * @param startT        the t parameter which determines the position of
         *                      the point {@code newA}
         * @param endT          the t parameter which determines the position of
         *                      the point {@code newB}
         * @return the coordinate of V
         */
        static QPointF calculateV(
                QPointF newA, QPointF newB,
                QPointF A, QPointF controlPointA,
                QPointF controlPointB, QPointF B,
                double startT, double endT);

        /**
         * Find the center O of a circle which makes the point {@code A} and the point
         * {@code G} on the circle and the line OA is vertical to the line AV.
         *
         * @param A the first point
         * @param V the second point
         * @param G the third point
         * @return the center of the circle
         */
        static QPointF calculateCenterOfArc(
                QPointF A, QPointF V, QPointF G);

        /**
         * This method is to calculate the max error between the original Bezier
         * curve and the fitted arcs.
         *
         * @param A             the start point of the Bezier curve
         * @param controlPointA the first control point of the Bezier curve which
         *                      is close to {@code A}
         * @param controlPointB the second control point of the Bezier curve which
         *                      is close to {@code B}
         * @param B             the end point of the Bezier curve
         * @param G             the incenter point
         * @param t             the t value which makes f(t) = (Q(t) - G) · H = 0
         * @return the max error between the original Bezier curve and the fitted
         * arcs.
         */
        static double calculateMaxError(
                QPointF A, QPointF controlPointA,
                QPointF controlPointB, QPointF B,
                QPointF G, double t);

        /**
         * To generate an {@code Arc} object according to the center, start point
         * and the end point.
         *
         * @param center     the center of the circle which makes the current arc on it
         * @param startPoint the start point of the arc
         * @param endPoint   the end point of the arc
         * @return an Arc object
         */
        static Arc generateArc(
                QPointF center, QPointF startPoint, QPointF endPoint);
};

#endif // BEZIERCURVETOARCS_H
