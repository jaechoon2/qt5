/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "qgeoprojection_p.h"

#include "qgeocoordinate.h"

#include <QMatrix4x4>
#include <qnumeric.h>

#include <cmath>

#include "qdoublevector2d_p.h"
#include "qdoublevector3d_p.h"

QT_BEGIN_NAMESPACE

QDoubleVector2D QGeoProjection::coordToMercator(const QGeoCoordinate &coord)
{
    const double pi = M_PI;

    double lon = coord.longitude() / 360.0 + 0.5;

    double lat = coord.latitude();
    lat = 0.5 - (std::log(std::tan((pi / 4.0) + (pi / 2.0) * lat / 180.0)) / pi) / 2.0;
    lat = qMax(0.0, lat);
    lat = qMin(1.0, lat);

    return QDoubleVector2D(lon, lat);
}

double QGeoProjection::realmod(const double a, const double b)
{
    quint64 div = static_cast<quint64>(a / b);
    return a - static_cast<double>(div) * b;
}

QGeoCoordinate QGeoProjection::mercatorToCoord(const QDoubleVector2D &mercator)
{
    const double pi = M_PI;

    double fx = mercator.x();
    double fy = mercator.y();

    if (fy < 0.0)
        fy = 0.0;
    else if (fy > 1.0)
        fy = 1.0;

    double lat;

    if (fy == 0.0)
        lat = 90.0;
    else if (fy == 1.0)
        lat = -90.0;
    else
        lat = (180.0 / pi) * (2.0 * std::atan(std::exp(pi * (1.0 - 2.0 * fy))) - (pi / 2.0));

    double lng;
    if (fx >= 0) {
        lng = realmod(fx, 1.0);
    } else {
        lng = realmod(1.0 - realmod(-1.0 * fx, 1.0), 1.0);
    }

    lng = lng * 360.0 - 180.0;

    return QGeoCoordinate(lat, lng, 0.0);
}

QT_END_NAMESPACE
