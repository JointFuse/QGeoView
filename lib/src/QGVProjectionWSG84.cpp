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

#include "QGVProjectionWSG84.h"
#include "WGS84toCartesian.hpp"

#include <QLineF>
#include <QtMath>

QGVProjectionWSG84::QGVProjectionWSG84()
    : QGVProjection("WSG84",
                    "WGS84 Web Mercator",
                    "QGVProjection used in web mapping applications like "
                    "Yandex/etc.")
{
    mEarthRadius = 6378137.0; /* meters */
    mGeoBoundary = QGV::GeoRect(85, -180, -85, +180);
    mProjBoundary = geoToProj(mGeoBoundary);
}

QGV::GeoRect QGVProjectionWSG84::boundaryGeoRect() const
{
    return mGeoBoundary;
}

QRectF QGVProjectionWSG84::boundaryProjRect() const
{
    return mProjBoundary;
}

QPointF QGVProjectionWSG84::geoToProj(const QGV::GeoPos& geoPos) const
{
    const double lon = geoPos.longitude();
    const double lat = (geoPos.latitude() > mGeoBoundary.topLeft().latitude()) ? mGeoBoundary.topLeft().latitude()
                                                                               : geoPos.latitude();
    const auto cartesian = wgs84::toCartesian({ 0, 0 }, { lat, lon });
    return QPointF(cartesian[1], cartesian[0]);
}

QGV::GeoPos QGVProjectionWSG84::projToGeo(const QPointF& projPos) const
{
    const auto cartesian = wgs84::fromCartesian({ 0, 0 }, { projPos.x(), projPos.y() });
    return QGV::GeoPos(cartesian[0], cartesian[1]);
}

QRectF QGVProjectionWSG84::geoToProj(const QGV::GeoRect& geoRect) const
{
    QRectF rect;
    rect.setTopLeft(geoToProj(geoRect.topLeft()));
    rect.setBottomRight(geoToProj(geoRect.bottomRight()));
    return rect;
}

QGV::GeoRect QGVProjectionWSG84::projToGeo(const QRectF& projRect) const
{
    return QGV::GeoRect(projToGeo(projRect.topLeft()), projToGeo(projRect.bottomRight()));
}

double QGVProjectionWSG84::geodesicMeters(const QPointF& projPos1, const QPointF& projPos2) const
{
    const QGV::GeoPos geoPos1 = projToGeo(projPos1);
    const QGV::GeoPos geoPos2 = projToGeo(projPos2);
    const double latitudeArc = (geoPos1.latitude() - geoPos2.latitude()) * M_PI / 180.0;
    const double longitudeArc = (geoPos1.longitude() - geoPos2.longitude()) * M_PI / 180.0;
    const double latitudeH = qPow(sin(latitudeArc * 0.5), 2);
    const double lontitudeH = qPow(sin(longitudeArc * 0.5), 2);
    const double lonFactor = cos(geoPos1.latitude() * M_PI / 180.0) * cos(geoPos2.latitude() * M_PI / 180.0);
    const double arcInRadians = 2.0 * asin(sqrt(latitudeH + lonFactor * lontitudeH));
    return mEarthRadius * arcInRadians;
}
