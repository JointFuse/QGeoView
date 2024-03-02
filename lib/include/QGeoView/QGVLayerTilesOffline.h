#ifndef QGVLAYERTILESOFFLINE_H
#define QGVLAYERTILESOFFLINE_H

#include "QGVLayerTiles.h"

class QGV_LIB_DECL QGVLayerTilesOffline : public QGVLayerTiles
{
    Q_OBJECT

protected:
    virtual QString tilePosToUrl(const QGV::GeoTilePos& tilePos) const = 0;

private:
    void onProjection(QGVMap* geoMap) override;
    void onClean() override {}
    void request(const QGV::GeoTilePos& tilePos) override;
    void cancel(const QGV::GeoTilePos&) override {}

};

#endif // QGVLAYERTILESOFFLINE_H
