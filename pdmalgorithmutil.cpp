#include "pdmalgorithmutil.h"
#include <qmath.h>
#include <QLineF>

PdmAlgorithmUtil::PdmAlgorithmUtil()
{

}

QPointF PdmAlgorithmUtil::getPosByCircleAngle(qreal iCx, qreal iCy, qreal iRadius, qreal iAngle, bool isMinusY) {
    qreal x = cos(iAngle / 360 * 2 * M_PI) * iRadius;
    qreal y = sin(iAngle / 360 * 2 * M_PI) * iRadius;
    if (isMinusY) {
        y = -y;
    }
    return QPointF(iCx + x, iCy + y);
}
