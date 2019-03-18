#include "painterpath2gerber.h"
#include <QRegularExpression>
#include "pdmalgorithmutil.h"
#include "beziercurve2arcs/beziercurvetoarcs.h"

const QRegularExpression expX("X([+-]?\\d+)");
const QRegularExpression expY("Y([+-]?\\d+)");

PainterPath2Gerber::PainterPath2Gerber()
{
}

QString PainterPath2Gerber::path2GerberStr(const QPainterPath &iPath)
{
    QPointF lastPos;
    mGerberStr.append("%FSTAX34Y34*%");
    mGerberStr.append("%MOIN*%");
    mGerberStr.append("%ADD10C,0.00100*%");
    mGerberStr.append("G75*");
    mGerberStr.append("G54D10*");
    for (int i = 0; i < iPath.elementCount(); ++i) {
        QPainterPath::Element element = iPath.elementAt(i);
        QPointF pos(element.x, element.y);
        if (element.type == QPainterPath::CurveToElement) {
            QPointF controlPosA = pos;
            QPainterPath::Element controlElement = iPath.elementAt(++i);
            QPointF controlPosB(controlElement.x, controlElement.y);
            controlElement = iPath.elementAt(++i);
            pos = QPointF(controlElement.x, controlElement.y);
            if (QLineF(lastPos, pos).length() > 0.01) {
                QVector<Arc> arcs = BezierCurveToArcs::convertACubicBezierCurveToArcs(lastPos, controlPosA, controlPosB, pos, 0.01);
                for (Arc arc: arcs) {
                    addGerberArc(arc.center.x(), arc.center.y(), arc.radius, arc.startAngle / M_PI * 180,
                                 arc.endAngle / M_PI * 180, arc.clockwiseFlag ? "G02" : "G03");
                }
            }
        } else if (element.type == QPainterPath::LineToElement) {
            addGerberLine(lastPos.x(), lastPos.y(), pos.x(), pos.y());
        }
        lastPos = pos;
    }
    mGerberStr.append("M02*");
    return mGerberStr.join("\n");
}

QString PainterPath2Gerber::doubleToStr(qreal iNum, int iPrecision)
{
    QString numStr = QString::number(iNum, 'f', iPrecision);
    if (numStr.contains(".")) {
        numStr = numStr.replace(QRegularExpression("\\.*0+$"), "");
    }
    return numStr;
}

QString PainterPath2Gerber::prependZeroByDecimals(const QString &iNumber, int iDecimals) {
    QRegularExpression signExp("[+-]");
    QRegularExpressionMatch match = signExp.match(iNumber);
    QString signStr = match.hasMatch() ? match.captured() : "";
    QString numberStr = iNumber;
    numberStr = numberStr.replace(signStr, "");
    int zeroCount = iDecimals - numberStr.length();
    QString str;
    while (zeroCount--) {
        str += "0";
    }
    return signStr + str + numberStr;
}

QString PainterPath2Gerber::getNumberStr(qreal iNumber)
{
    qreal number = iNumber / 25.4;
    QStringList numberStr = doubleToStr(number, 4).split(".");
    QString numberIntStr = prependZeroByDecimals(numberStr.first(), 3);
    if (numberStr.count() == 2) {
        numberIntStr += numberStr.last();
    }
    return numberIntStr;
}

QString PainterPath2Gerber::getSiteStr(qreal iX, qreal iY)
{
    QString xStr = getNumberStr(iX);
    QString yStr = getNumberStr(iY);
    return QString("X%1Y%2").arg(xStr).arg(yStr);
}

bool PainterPath2Gerber::getNumber(const QString &iStr, const QRegularExpression &iReg, qreal *oNumber)
{
    QRegularExpressionMatch regMatch = iReg.match(iStr);
    QString matchStr = regMatch.hasMatch() ? regMatch.captured() : "";
    bool result = false;
    if (!matchStr.isEmpty()) {
        QRegularExpression numberReg("[+-]?\\d+");
        regMatch = numberReg.match(matchStr);
        matchStr = regMatch.hasMatch() ? regMatch.captured() : "";
        if (!matchStr.isEmpty()) {
            *oNumber = matchStr.toDouble();
            result = true;
        }
    }
    return result;
}

bool PainterPath2Gerber::siteIsEquality(const QString &iSite1, const QString &iSite2)
{
    qreal x1,x2,y1,y2 = 0;
    qreal result = false;
    if (getNumber(iSite1, expX, &x1) && getNumber(iSite1, expY, &y1)
            &&getNumber(iSite2, expX, &x2) && getNumber(iSite2, expY, &y2)) {
        result = (x1 == x2) && (y1==y2);
    }
    return result;
}

void PainterPath2Gerber::addGerberLine(qreal iX1, qreal iY1, qreal iX2, qreal iY2)
{
    QString lastSite = getSiteStr(iX1, iY1);
    if (siteIsEquality(lastSite, mGerberStr.last()) == false) {
        mGerberStr.append(QString("%1D02*").arg(lastSite));
    }
    QString site = getSiteStr(iX2, iY2);
    if (site != lastSite) {
        mGerberStr.append(QString("G01%1D01*").arg(site));
    }
}

void PainterPath2Gerber::addGerberArc(qreal iCx, qreal iCy, qreal iRadius, qreal iStartAngle, qreal iEndAngle, const QString &iType)
{
    QPointF startPos = PdmAlgorithmUtil::getPosByCircleAngle(iCx, iCy, iRadius, iStartAngle);
    QPointF endPos = PdmAlgorithmUtil::getPosByCircleAngle(iCx, iCy, iRadius, iEndAngle);
    addGerberArc(QPointF(iCx, iCy), startPos, endPos, iType);
}

void PainterPath2Gerber::addGerberArc(const QPointF &iCenter, const QPointF &iStartPos, const QPointF &iEndPos, const QString &iType)
{
    QString lastSite = getSiteStr(iStartPos.x(), iStartPos.y());
    if (siteIsEquality(lastSite, mGerberStr.last()) == false) {
        mGerberStr.append(QString("%1D02*").arg(lastSite));
    }
    QString site = getSiteStr(iEndPos.x(), iEndPos.y());
    if (site != lastSite) {
        qreal i = iCenter.x() - iStartPos.x();
        qreal j = iCenter.y() - iStartPos.y();
        mGerberStr.append(QString("%1%2I%3J%4D01*").arg(iType).arg(site)
                      .arg(getNumberStr(i)).arg(getNumberStr(j)));
    }
}
