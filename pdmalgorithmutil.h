#ifndef PDMALGORITHMUTIL_H
#define PDMALGORITHMUTIL_H

#include <QPointF>

class PdmAlgorithmUtil
{
public:
    PdmAlgorithmUtil();
    static QPointF getPosByCircleAngle(qreal iCx, qreal iCy, qreal iRadius, qreal iAngle, bool isMinusY = false);
};

#endif // PDMALGORITHMUTIL_H
