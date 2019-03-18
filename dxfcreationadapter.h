#ifndef CUSTOM_DXF_CREATION_ADAPTER_H
#define CUSTOM_DXF_CREATION_ADAPTER_H

#include <QStringList>
#include <QMap>
#include <QPointF>
#include <QPainterPath>
#include "thirdparty/dxflib/dl_creationadapter.h"

struct GraphicsItem {
    QString name;
    QPointF pos;
    /*! X Scale factor. */
    qreal sx = 1;
    /*! Y Scale factor. */
    qreal sy = 1;
    /*! Rotation angle in degrees. */
    qreal angle = 0;
};

struct GraphicsPrimitive {
    QString name;
    QPainterPath path;
    QVector<GraphicsItem> items;
};

class DxfCreationAdapter : public DL_CreationAdapter
{
public:
    enum ItemMode {
        NoneMode,
        Polyline
    };
    DxfCreationAdapter();
    QMap<QString, GraphicsPrimitive> getAllLayers();
    QMap<QString, GraphicsPrimitive> getAllBlock();
    GraphicsPrimitive getBlock(const QString &iName);

    void addLayer(const DL_LayerData &iData) override;

    void addPoint(const DL_PointData &iData) override;
    void addLine(const DL_LineData &iData) override;

    void addArc(const DL_ArcData &iData) override;
    void addCircle(const DL_CircleData &iData) override;
    void addEllipse(const DL_EllipseData &iData) override;

    void addPolyline(const DL_PolylineData &iData) override;
    void addVertex(const DL_VertexData &iData) override;

    virtual void addBlock(const DL_BlockData &iData) override;
    virtual void endBlock() override;

    virtual void endSection();
    virtual void endEntity();

    virtual void addInsert(const DL_InsertData &iData);

    void printAttributes();

    GraphicsPrimitive &getGraphicsPrimitive(const QString &iPrimitiveName);
    void addPrimitiveLine(const QString &iPrimitiveName, qreal iX1, qreal iY1, qreal iX2, qreal iY2);
    void addPrimitiveArc(const QString &iPrimitiveName, qreal iCx, qreal iCy, qreal iRadius,
                      qreal iStartAngle, qreal iEndAngle, const QString &iType = "G03");
    void addPrimitiveArc(const QString &iPrimitiveName, const QPointF &iCenter, const QPointF &iStartPos,
                      const QPointF &iEndPos, const QString &iType = "G03");
    void addPrimitivePolyline(const QPointF &iStartPos, const QPointF &iEndPos, qreal iBluge);

private:
    QString mBlockName;
    DL_VertexData mFirstVertex;
    DL_VertexData mLastVertex;
    bool mIsFirstVertex = true;
    bool mIsClosePoly = false;
    ItemMode mCurrentMode = NoneMode;
    QMap<QString, GraphicsPrimitive> mLayers;
    QMap<QString, GraphicsPrimitive> mBlockItems;
};

#endif // CUSTOM_DXF_CREATION_ADAPTER_H
