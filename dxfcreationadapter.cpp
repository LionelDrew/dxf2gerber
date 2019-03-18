#include "dxfcreationadapter.h"
#include <stdio.h>
#include <QDebug>
#include <QLineF>
#include <QTransform>
#include <QPainter>
#include <QLabel>
#include <QApplication>
#include <QRegularExpression>
#include "pdmalgorithmutil.h"
#include "beziercurve2arcs/beziercurvetoarcs.h"

DxfCreationAdapter::DxfCreationAdapter()
{
}

QMap<QString, GraphicsPrimitive> DxfCreationAdapter::getAllLayers()
{
    return mLayers;
}

QMap<QString, GraphicsPrimitive> DxfCreationAdapter::getAllBlock()
{
    return mBlockItems;
}

GraphicsPrimitive DxfCreationAdapter::getBlock(const QString &iName)
{
    return mBlockItems.value(iName);
}

void DxfCreationAdapter::addLayer(const DL_LayerData &iData)
{
    QString name = iData.name.c_str();
    GraphicsPrimitive primitive;
    primitive.name = name;
    mLayers[name] = primitive;
}

void DxfCreationAdapter::addPoint(const DL_PointData &iData)
{
    Q_UNUSED(iData);
}

void DxfCreationAdapter::addLine(const DL_LineData &iData)
{
    addPrimitiveLine(attributes.getLayer().c_str(), iData.x1, iData.y1, iData.x2, iData.y2);
}

void DxfCreationAdapter::addArc(const DL_ArcData &iData)
{
    QPointF startPos = PdmAlgorithmUtil::getPosByCircleAngle(iData.cx, iData.cy, iData.radius, iData.angle1);
    QPointF endPos = PdmAlgorithmUtil::getPosByCircleAngle(iData.cx, iData.cy, iData.radius, iData.angle2);
    addPrimitiveArc(attributes.getLayer().c_str(), QPointF(iData.cx, iData.cy), startPos, endPos);
}

void DxfCreationAdapter::addCircle(const DL_CircleData &iData)
{
    addPrimitiveArc(attributes.getLayer().c_str(), iData.cx, iData.cy, iData.radius, 0, 0);
}

void DxfCreationAdapter::addEllipse(const DL_EllipseData &iData)
{
    QPointF cPoint(iData.cx, iData.cy);
    QPointF sPoint = cPoint + QPointF(iData.mx, iData.my);
    QLineF sLine(cPoint, sPoint);
    qreal r1 = sLine.length();
    qreal r2 = r1 * iData.ratio;
    qreal alpha = sLine.angle();
    QPainterPath path;
    qreal startAngle = iData.angle1 * 180 / M_PI;
    qreal endAngle = iData.angle2 * 180 / M_PI;
    if (startAngle > endAngle) {
        endAngle += 360;
    }
    path.arcTo(-r1, -r2, 2 * r1, 2 * r2, startAngle, endAngle - startAngle);
    QTransform trans;
    trans.scale(1,-1);
    trans.translate(cPoint.x(), -cPoint.y());
    trans.rotate(alpha);
    path = trans.map(path);

    GraphicsPrimitive &primitive = getGraphicsPrimitive(attributes.getLayer().c_str());
    QPointF lastPos;
    for (int i = 0; i < path.elementCount(); ++i) {
        QPainterPath::Element element = path.elementAt(i);
        QPointF pos(element.x, element.y);
        if (element.type == QPainterPath::CurveToElement) {
            QPointF controlPosA = pos;
            QPainterPath::Element controlElement = path.elementAt(++i);
            QPointF controlPosB(controlElement.x, controlElement.y);
            controlElement = path.elementAt(++i);
            pos = QPointF(controlElement.x, controlElement.y);
            primitive.path.moveTo(lastPos);
            primitive.path.cubicTo(controlPosA, controlPosB, pos);
        }
        lastPos = pos;
    }
}

void DxfCreationAdapter::addPolyline(const DL_PolylineData &iData)
{
    Q_UNUSED(iData);
    mIsFirstVertex = true;
    mCurrentMode = Polyline;
    mIsClosePoly = iData.flags == 1;
}

void DxfCreationAdapter::addVertex(const DL_VertexData &iData)
{
    if (mIsFirstVertex) {
        mIsFirstVertex = false;
        mFirstVertex = iData;
        GraphicsPrimitive &primitive = getGraphicsPrimitive(attributes.getLayer().c_str());
        primitive.path.moveTo(iData.x, iData.y);
    } else {
        QPointF startPos(mLastVertex.x, mLastVertex.y);
        QPointF endPos(iData.x, iData.y);
        qreal bluge = mLastVertex.bulge;
        addPrimitivePolyline(startPos, endPos, bluge);
    }
    mLastVertex = iData;
}

void DxfCreationAdapter::addBlock(const DL_BlockData &iData)
{
    mBlockName = iData.name.c_str();
    if (mBlockName == "1") {
        int i=0;
        ++i;
    }
    GraphicsPrimitive primitive;
    primitive.name = mBlockName;
    mBlockItems[mBlockName] = primitive;
}

void DxfCreationAdapter::endBlock()
{
    mBlockName.clear();
}

void DxfCreationAdapter::endSection()
{
}

void DxfCreationAdapter::endEntity()
{
    if (mCurrentMode == Polyline && mIsClosePoly) {
        QPointF startPos(mLastVertex.x, mLastVertex.y);
        QPointF endPos(mFirstVertex.x, mFirstVertex.y);
        qreal bluge = mLastVertex.bulge;
        addPrimitivePolyline(startPos, endPos, bluge);
    }
    mCurrentMode = NoneMode;
}

void DxfCreationAdapter::addInsert(const DL_InsertData &iData)
{
    GraphicsPrimitive &primitive = getGraphicsPrimitive(attributes.getLayer().c_str());
    GraphicsItem item;
    item.name = iData.name.c_str();
    item.angle = iData.angle;
    item.pos = QPointF(iData.ipx, iData.ipy);
    item.sx = iData.sx;
    item.sy = iData.sy;
    primitive.items.append(item);
}

void DxfCreationAdapter::printAttributes()
{
    printf("  Attributes: Layer: %s, ", attributes.getLayer().c_str());
    printf(" Color: ");
    if (attributes.getColor()==256)	{
        printf("BYLAYER");
    } else if (attributes.getColor()==0) {
        printf("BYBLOCK");
    } else {
        printf("%d", attributes.getColor());
    }
    printf(" Width: ");
    if (attributes.getWidth()==-1) {
        printf("BYLAYER");
    } else if (attributes.getWidth()==-2) {
        printf("BYBLOCK");
    } else if (attributes.getWidth()==-3) {
        printf("DEFAULT");
    } else {
        printf("%d", attributes.getWidth());
    }
    printf(" Type: %s\n", attributes.getLinetype().c_str());
}

GraphicsPrimitive &DxfCreationAdapter::getGraphicsPrimitive(const QString &iPrimitiveName)
{
    if (mBlockName.isEmpty()) {
        return mLayers[iPrimitiveName];
    } else {
        return mBlockItems[mBlockName];
    }
}

void DxfCreationAdapter::addPrimitiveLine(const QString &iPrimitiveName, qreal iX1, qreal iY1, qreal iX2, qreal iY2)
{
    GraphicsPrimitive &primitive = getGraphicsPrimitive(iPrimitiveName);
    primitive.path.moveTo(iX1, iY1);
    primitive.path.lineTo(iX2, iY2);
}

void DxfCreationAdapter::addPrimitiveArc(const QString &iPrimitiveName, qreal iCx, qreal iCy, qreal iRadius,
                                         qreal iStartAngle, qreal iEndAngle, const QString &iType)
{
    GraphicsPrimitive &primitive = getGraphicsPrimitive(iPrimitiveName);
    if (iEndAngle < iStartAngle) {
        iEndAngle += 360;
    }
    qreal arcLength = iEndAngle - iStartAngle;
    if (iType == "G03") {
        arcLength -= 360;
    }
    primitive.path.moveTo(PdmAlgorithmUtil::getPosByCircleAngle(iCx, iCy, iRadius, iStartAngle, true));
    primitive.path.arcTo(QRectF(iCx-iRadius,iCy-iRadius,2*iRadius,2*iRadius), iStartAngle, arcLength);
}

void DxfCreationAdapter::addPrimitiveArc(const QString &iPrimitiveName, const QPointF &iCenter, const QPointF &iStartPos,
                                         const QPointF &iEndPos, const QString &iType)
{
    addPrimitiveArc(iPrimitiveName, iCenter.x(), iCenter.y(), QLineF(iCenter, iEndPos).length(),
                 QLineF(iCenter, iStartPos).angle(), QLineF(iCenter, iEndPos).angle(), iType);
}

void DxfCreationAdapter::addPrimitivePolyline(const QPointF &iStartPos, const QPointF &iEndPos, qreal iBluge)
{
    qreal bluge = iBluge;
    QPointF startPos = iStartPos;
    QPointF endPos = iEndPos;
    if (bluge == 0 || QLineF(iStartPos, iEndPos).length() < 0.1) {
        addPrimitiveLine(attributes.getLayer().c_str(), startPos.x(), startPos.y(), endPos.x(), endPos.y());
    } else {
        qreal dis = QLineF(startPos, endPos).length();
        qreal radius = (dis / 4) * (bluge + (1/bluge));
        //已知半径和两点求圆心
        qreal R = qAbs(radius);
        qreal x1 = startPos.x();
        qreal y1= startPos.y();
        qreal x2 = endPos.x();
        qreal y2 = endPos.y();
        double c1 = (x2*x2 - x1*x1 + y2*y2 - y1*y1) / (2 *(x2 - x1));
        double c2 = (y2 - y1) / (x2 - x1);  //斜率
        double A = (c2*c2 + 1);
        double B = (2 * x1*c2 - 2 * c1*c2 - 2 * y1);
        double C = x1*x1 - 2 * x1*c1 + c1*c1 + y1*y1 - R*R;
        qreal cy1 = (-B + sqrt(B*B - 4 * A*C)) / (2 * A);
        qreal cy2 = (-B - sqrt(B*B - 4 * A*C)) / (2 * A);
        qreal y = 0;
        qreal tolerance = 0.0001;
        if (startPos.x() + tolerance < endPos.x()) {
            y = cy1 > cy2 ? cy1 : cy2;
        } else if (startPos.x() - tolerance > endPos.x()) {
            y = cy1 < cy2 ? cy1 : cy2;
        } else {
            qreal cx1 = c1 - c2 * cy1;
            qreal cx2 = c1 - c2 * cy2;
            if (startPos.y() < endPos.y()) {
                y = cx1 > cx2 ? cy1 : cy2;
            } else {
                y = cx1 < cx2 ? cy1 : cy2;
            }
        }
        QString type = "G03";
        if (bluge < 0) {
            y = y == cy1 ? cy2 : cy1;
            type = "G02";
        }
        qreal x = c1 - c2 * y;
        addPrimitiveArc(attributes.getLayer().c_str(), QPointF(x, y), startPos, endPos, type);
    }
}

