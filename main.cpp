#include <QApplication>
#include <QFile>
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsPathItem>
#include <QDebug>
#include "thirdparty/dxflib/dl_dxf.h"
#include "painterpath2gerber.h"

QPainterPath getGraphicsItemPath(const GraphicsPrimitive &iItem, const QMap<QString, GraphicsPrimitive> &iBlockItems) {
    QPainterPath path = iItem.path;
    for (GraphicsItem item: iItem.items) {
        QPainterPath itemPath = getGraphicsItemPath(iBlockItems[item.name], iBlockItems);
        QTransform trans;
        trans.translate(item.pos.x(), item.pos.y());
        trans.rotate(item.angle);
        trans.scale(item.sx, item.sy);
        path.addPath(trans.map(itemPath));
    }
    return path;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DxfCreationAdapter *creationAdapter = new DxfCreationAdapter();
    DL_Dxf *dxf = new DL_Dxf();
    QFile file("d:\\demo.dxf");
    if (file.open(QFile::ReadOnly)) {
        std::stringstream stream;
        stream << file.readAll().toStdString();
        if (!dxf->in(stream, creationAdapter)) {
            std::cerr << "could not be opened.\n";
        }
        QString dir = "c:";
        QMap<QString, GraphicsPrimitive> layers = creationAdapter->getAllLayers();
        QMap<QString, GraphicsPrimitive> blocks = creationAdapter->getAllBlock();
        QMapIterator<QString, GraphicsPrimitive> i(layers);
        QGraphicsView *view = new QGraphicsView;
        QGraphicsScene *scene = new QGraphicsScene;
        view->setScene(scene);
        while (i.hasNext()) {
            i.next();
            QPainterPath path = getGraphicsItemPath(i.value(), blocks);
            if (!path.isEmpty()) {
                QFile file(QString("%1\\%2.gbr").arg(dir).arg(QString(i.key().toLocal8Bit())));
                if (file.open(QFile::WriteOnly)) {
                    file.write(PainterPath2Gerber().path2GerberStr(path).toUtf8());
                }
                QTransform trans;
                trans.scale(3,3);
                path = trans.map(path);
                scene->addItem(new QGraphicsPathItem(path));
            }
        }
        view->show();
    }
    delete dxf;
    delete creationAdapter;
//    return a.exec();
    return 0;
}
