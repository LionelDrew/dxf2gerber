#ifndef DXF2GERBERUTIL_H
#define DXF2GERBERUTIL_H

#include "dxfcreationadapter.h"

class PainterPath2Gerber
{
public:
    PainterPath2Gerber();
    QString path2GerberStr(const QPainterPath &iPath);
    QString doubleToStr(qreal iNum, int iPrecision);
    QString prependZeroByDecimals(const QString &iNumber, int iDecimals);
    QString getNumberStr(qreal iNumber);
    QString getSiteStr(qreal iX, qreal iY);
    bool getNumber(const QString &iStr, const QRegularExpression &iReg, qreal *oNumber);
    bool siteIsEquality(const QString &iSite1, const QString &iSite2);
    void addGerberLine(qreal iX1, qreal iY1, qreal iX2, qreal iY2);
    void addGerberArc(qreal iCx, qreal iCy, qreal iRadius, qreal iStartAngle, qreal iEndAngle, const QString &iType = "G03");
    void addGerberArc(const QPointF &iCenter, const QPointF &iStartPos, const QPointF &iEndPos, const QString &iType = "G03");
private:
    QStringList mGerberStr;
};

#endif // DXF2GERBERUTIL_H
