#include "beziercurvetoarcs.h"
#include "cubicbeziertools.h"
#include <QDebug>

BezierCurveToArcs::BezierCurveToArcs()
{

}

QVector<Arc> BezierCurveToArcs::convertACubicBezierCurveToArcs(const QPointF &iPosA, const QPointF &iControlPointA,
                                                               const QPointF &iControlPointB, const QPointF &iPosB, double iAllowableError)
{
    return convertACubicBezierCurveToArcsHelper(iPosA, iControlPointA, iControlPointB, iPosB, 0.0, 1.0, iAllowableError);
}

QVector<Arc> BezierCurveToArcs::convertACubicBezierCurveToArcsHelper(const QPointF &iPosA, const QPointF &iControlPointA,
                                                                     const QPointF &iControlPointB, const QPointF &iPosB,
                                                                     qreal iStartT, qreal iEndT, double iAllowableError)
{
    QVector<Arc> result;
    /* Step 1: Calculate the new start point and the new end point of the
    current circumstance */
    QPointF A0 = CubicBezierTools::pointOnBezierCurve(iStartT, iPosA, iControlPointA, iControlPointB, iPosB);
    QPointF A1 = CubicBezierTools::pointOnBezierCurve(iEndT, iPosA, iControlPointA, iControlPointB, iPosB);
    /* Step 2: Calculate the intersection of the two tangent lines from the
    new start point and the end point*/
    QPointF V = calculateV(A0, A1, iPosA, iControlPointA, iControlPointB, iPosB,
                    iStartT, iEndT);

    /* Step 3: Find the incenter of the triangle A0A1V */
    QPointF G = MathTools::findInCenterPointOfTriangle(A0, V, A1);

    /* Step 4: Find two centers of biarc */
    QPointF center1 = calculateCenterOfArc(A0, V, G);
    QPointF center2 = calculateCenterOfArc(A1, V, G);

    /* Step 5: Calculate the unit tangent vector of the circle on point G */
    QPointF H = MathTools::calculateUnitTangentVectorOfCircle(center1, G);

    /* Step 6: Calculate t */
    double t = findTWithNewtonAndRaphsonMethod(iPosA, iControlPointA, iControlPointB, iPosB,
                                                H, G, AllOWABLE_ERROR_FOR_FIND_T, iStartT, iEndT);

    /* Step 7: Calculate max error between the fitted arcs and the original
    Bezier curve */
    double maxError = calculateMaxError(iPosA, iControlPointA, iControlPointB, iPosB, G, t);

    /* Step 8: Judge if the current approximation meets the allowable error */
    if (maxError <= iAllowableError) {
        Arc firstArc = generateArc(center1, A0, G);
        Arc secondArc = generateArc(center2, G, A1);
        result.append(firstArc);
        result.append(secondArc);

    } else {
        result.append(convertACubicBezierCurveToArcsHelper(iPosA, iControlPointA, iControlPointB, iPosB, iStartT, t, iAllowableError));
        result.append(convertACubicBezierCurveToArcsHelper(iPosA, iControlPointA, iControlPointB, iPosB, t, iEndT, iAllowableError));
    }
    return result;
}

double BezierCurveToArcs::findTWithNewtonAndRaphsonMethod(QPointF A, QPointF controlPointA, QPointF controlPointB, QPointF B, QPointF H, QPointF G, double allowableError, double startT, double endT) {

    double tn = startT + (endT - startT) / 2.0;

    QPointF Q_tn = CubicBezierTools::pointOnBezierCurve(tn,
                                                       A, controlPointA, controlPointB, B);

    double fn = MathTools::dotProductOfTwoPoints(Q_tn, H)
            - MathTools::dotProductOfTwoPoints(G, H);

    while (abs(fn) > allowableError) {

        QPointF d_Q_tn
                = CubicBezierTools::calculateDerivativeOnBezierCurve(
                    A, controlPointA, controlPointB, B, tn);

        double d_fn = MathTools::dotProductOfTwoPoints(d_Q_tn, H);

        tn = tn - fn / d_fn;

        Q_tn = CubicBezierTools::pointOnBezierCurve(tn,
                                                   A, controlPointA, controlPointB, B);

        fn = MathTools::dotProductOfTwoPoints(Q_tn, H)
                - MathTools::dotProductOfTwoPoints(G, H);
    }

    return tn;
}

QPointF BezierCurveToArcs::calculateV(QPointF newA, QPointF newB, QPointF A, QPointF controlPointA, QPointF controlPointB, QPointF B, double startT, double endT) {

    QPointF unitTangentVector0 = CubicBezierTools::calculateUnitTangentVectorOfBezierCurve(A, controlPointA, controlPointB, B, startT);

    QPointF unitTangentVector1
            = CubicBezierTools::calculateUnitTangentVectorOfBezierCurve(
                A, controlPointA, controlPointB, B, endT);

    QPointF adjacentNewA = QPointF(newA.x() + unitTangentVector0.x(),
                          newA.y() + unitTangentVector0.y());

    QPointF adjacentNewB = QPointF(newB.x() + unitTangentVector1.x(),
                          newB.y() + unitTangentVector1.y());

    return MathTools::calculateIntersectionOfTwoLine(newA, adjacentNewA, adjacentNewB, newB);
}

QPointF BezierCurveToArcs::calculateCenterOfArc(QPointF A, QPointF V, QPointF G) {

    double a11 = V.x() - A.x();
    double a12 = V.y() - A.y();
    double b1 = A.x() * a11 + A.y() * a12;

    double a21 = G.x() - A.x();
    double a22 = G.y() - A.y();
    double b2 = (G.x() + A.x()) / 2.0 * a21
            + (G.y() + A.y()) / 2.0 * a22;

    return MathTools::solveLinearEquationsOfTwoUnknownVariables(
                a11, a12, b1, a21, a22, b2);
}

double BezierCurveToArcs::calculateMaxError(QPointF A, QPointF controlPointA, QPointF controlPointB, QPointF B, QPointF G, double t) {

    QPointF Q_t = CubicBezierTools::pointOnBezierCurve(t,
                                                      A, controlPointA, controlPointB, B);

    return MathTools::euclideanDistance(Q_t, G);
}

Arc BezierCurveToArcs::generateArc(QPointF center, QPointF startPoint, QPointF endPoint) {

    double radius = MathTools::euclideanDistance(center, endPoint);

    double startAngle
            = MathTools::calculateAngleRelativeToCircleCenter(center, startPoint);
    double endAngle
            = MathTools::calculateAngleRelativeToCircleCenter(center, endPoint);

    while (endAngle < startAngle - M_PI) {
        endAngle += 2.0 * M_PI;
    }
    while (endAngle > startAngle + M_PI) {
        endAngle -= 2.0 * M_PI;
    }

    if (startAngle <= endAngle) {
        return Arc(center, radius, startAngle, endAngle, false);
    } else {
        return Arc(center, radius, startAngle, endAngle, true);
    }
}
