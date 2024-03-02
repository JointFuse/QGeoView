/***************************************************************************
 * QGeoView is a Qt / C ++ widget for visualizing geographic data.
 * Copyright (C) 2018-2023 Andrey Yaroshenko.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, see https://www.gnu.org/licenses.
 ****************************************************************************/

#include "QGVLayerOsmAndTile.h"

#include <QtMath>

namespace {
// clang-format off
const auto URLTemplate = QStringLiteral("../${z}/${x}/${y}.png.tile");
// clang-format on
}

QGVLayerOsmAndTile::QGVLayerOsmAndTile()
    : mUrl(URLTemplate)
{
    setName("OpenStreetMap");
    setDescription("Copyrights ©OpenStreetMap");
}

QGVLayerOsmAndTile::QGVLayerOsmAndTile(const QString& url)
    : mUrl(url)
{
    setName("Custom");
    setDescription("OSM-like map");
}

void QGVLayerOsmAndTile::setUrl(const QString& url)
{
    mUrl = url;
}

QString QGVLayerOsmAndTile::getUrl() const
{
    return mUrl;
}

int QGVLayerOsmAndTile::minZoomlevel() const
{
    return 1;
}

int QGVLayerOsmAndTile::maxZoomlevel() const
{
    return 19;
}

QString QGVLayerOsmAndTile::tilePosToUrl(const QGV::GeoTilePos& tilePos) const
{
    QString url = mUrl;
    url.replace("${z}", QString::number(tilePos.zoom()));
    url.replace("${x}", QString::number(tilePos.pos().x()));
    url.replace("${y}", QString::number(tilePos.pos().y()));
    return url;
}
