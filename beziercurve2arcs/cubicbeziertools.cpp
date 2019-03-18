#include "cubicbeziertools.h"

CubicBezierTools::CubicBezierTools()
{

}


QPointF CubicBezierTools::pointOnBezierCurve(qreal iT, const QPointF &iPosA, const QPointF &iControlPointA, const QPointF &iControlPointB, const QPointF &iPosB)
{
    double s = 1 - iT;
    double x = s * s * s * iPosA.x() + 3 * (s * s * iT) * iControlPointA.x()
            + 3 * (iT * iT * s) * iControlPointB.x() + iT * iT * iT * iPosB.x();
    double y = s * s * s * iPosA.y() + 3 * (s * s * iT) * iControlPointA.y()
            + 3 * (iT * iT * s) * iControlPointB.y() + iT * iT * iT * iPosB.y();
    return QPointF(x, y);
}

QPointF CubicBezierTools::calculateUnitTangentVectorOfBezierCurve(const QPointF &iPosA, const QPointF &controlPointA, const QPointF &iControlPointB, const QPointF &iPosB, double iT) {
    double x0 = iPosA.x();
    double y0 = iPosA.y();
    double x1 = controlPointA.x();
    double y1 = controlPointA.y();
    double x2 = iControlPointB.x();
    double y2 = iControlPointB.y();
    double x3 = iPosB.x();
    double y3 = iPosB.y();

    double s = 1.0 - iT;

    double dy_dt = -3 * y0 * s * s + 3 * y1 * (s * s - 2 * iT * s)
            + 3 * y2 * (2 * iT * s - iT * iT) + 3 * y3 * iT * iT;
    double dx_dt = -3 * x0 * s * s + 3 * x1 * (s * s - 2 * iT * s)
            + 3 * x2 * (2 * iT * s - iT * iT) + 3 * x3 * iT * iT;

    if (abs(dx_dt) <= EPSILON) {

        if (dy_dt >= 0.0) {
            return QPointF(0.0, 1.0);
        } else {
            return QPointF(0.0, -1.0);
        }
    }

    double hypotenuse = sqrt(dx_dt * dx_dt + dy_dt * dy_dt);

    return QPointF(dx_dt / hypotenuse, dy_dt / hypotenuse);
}

QPointF CubicBezierTools::calculateDerivativeOnBezierCurve(const QPointF &iPosA, const QPointF &controlPointA, const QPointF &iControlPointB, const QPointF &iPosB, double iT) {
    double x0 = iPosA.x();
    double y0 = iPosA.y();
    double x1 = controlPointA.x();
    double y1 = controlPointA.y();
    double x2 = iControlPointB.x();
    double y2 = iControlPointB.y();
    double x3 = iPosB.x();
    double y3 = iPosB.y();
    double s = 1.0 - iT;

    double dy_dt = -3 * y0 * s * s + 3 * y1 * (s * s - 2 * iT * s)
            + 3 * y2 * (2 * iT * s - iT * iT) + 3 * y3 * iT * iT;
    double dx_dt = -3 * x0 * s * s + 3 * x1 * (s * s - 2 * iT * s)
            + 3 * x2 * (2 * iT * s - iT * iT) + 3 * x3 * iT * iT;

    return QPointF(dx_dt, dy_dt);
}
