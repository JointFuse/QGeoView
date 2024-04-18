#include "QGVLayerTilesOffline.h"
#include "QGVImage.h"
#include <QFile>

void QGVLayerTilesOffline::onProjection(QGVMap* geoMap)
{
    QGVLayerTiles::onProjection(geoMap);
}

void QGVLayerTilesOffline::request(const QGV::GeoTilePos& tilePos)
{
    QFile rawFile{ tilePosToUrl(tilePos) };

    if (!rawFile.exists() || !rawFile.open(QIODevice::ReadOnly))
        return;

    auto tile = new QGVImage();
    tile->setGeometry(tilePos.toGeoRect());
    tile->loadImage(rawFile.readAll());
    tile->setProperty("drawDebug",
                      QString("%1\ntile(%2,%3,%4)")
                              .arg(rawFile.fileName())
                              .arg(tilePos.zoom())
                              .arg(tilePos.pos().x())
                              .arg(tilePos.pos().y()));
    onTile(tilePos, tile);
}
