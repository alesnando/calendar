/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "sqleventmodel.h"

#include <QDebug>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QTime>

SqlEventModel::SqlEventModel() :
    QSqlQueryModel()
{
    createConnection();
}

QList<QObject*> SqlEventModel::eventsForDate(const QDate &date)
{
    const QString queryStr = QString::fromLatin1("SELECT * FROM Event WHERE '%1' >= startDate AND '%1' <= endDate order by startTime asc ").arg(date.toString("yyyy-MM-dd"));
    QSqlQuery query(queryStr);
    if (!query.exec())
        qFatal("Query failed");

    QList<QObject*> events;
    while (query.next()) {
        Event *event = new Event(this);
        event->setName(query.value("name").toString());

        QDateTime startDate;
        startDate.setDate(query.value("startDate").toDate());
        startDate.setTime(query.value("startTime").toTime());
        event->setStartDate(startDate);

        QDateTime endDate;
        endDate.setDate(query.value("endDate").toDate());
        endDate.setTime(query.value("endTime").toTime());
        event->setEndDate(endDate);

        events.append(event);
    }

    return events;
}

QList<QObject*> SqlEventModel::listaEventsForDate(const QDate &date)
{
    const QString queryStr = QString::fromLatin1("SELECT * FROM Event WHERE '%1' >= startDate AND '%1' <= endDate order by startTime asc ").arg(date.toString("yyyy-MM-dd"));
    QSqlQuery query(queryStr);
    if (!query.exec())
        qFatal("Query failed");

    QList<QObject*> events;
    while (query.next()) {
        Event *event = new Event(this);
        event->setName(query.value("name").toString());

        QDateTime startDate;
        startDate.setDate(query.value("startDate").toDate());
        startDate.setTime(query.value("startTime").toTime());
        event->setStartDate(startDate);

        QDateTime endDate;
        endDate.setDate(query.value("endDate").toDate());
        endDate.setTime(query.value("endTime").toTime());
        event->setEndDate(endDate);

        events.append(event);
    }

    return events;
}

bool SqlEventModel::nuevoEvent(QString nombre, QDateTime fechaInicio, QDateTime fechaFin)
{
    QTime horaInicio = fechaInicio.time();
    QTime horaFin = fechaFin.time();
    QSqlQuery query;
    // We store the time as seconds because it's easier to query.
    query.prepare("insert into Event values(:nombre, :fechaInicio, :horaInicio, :fechaFin, :horaFin)");
    query.bindValue(":nombre", nombre);
    query.bindValue(":fechaInicio", fechaInicio.date());
    query.bindValue(":horaInicio", horaInicio);
    query.bindValue(":fechaFin", fechaFin.date());
    query.bindValue(":horaFin", horaFin);
    bool inserto = query.exec();
    if(inserto){
        listaEventsForDate(fechaInicio.date());
    }
    return inserto;
}

QList<QObject*> SqlEventModel::eliminarEvent(const QDate &fecha)
{
    const QString queryStr = QString::fromLatin1("SELECT * FROM Event WHERE '%1' >= startDate AND '%1' <= endDate order by startTime asc ").arg(fecha.toString("yyyy-MM-dd"));
    QSqlQuery query(queryStr);
    if (!query.exec())
        qFatal("Query failed");

    QList<QObject*> events;
    while (query.next()) {
        Event *event = new Event(this);
        event->setName(query.value("name").toString());

        QDateTime startDate;
        startDate.setDate(query.value("startDate").toDate());
        startDate.setTime(query.value("startTime").toTime());
        event->setStartDate(startDate);

        QDateTime endDate;
        endDate.setDate(query.value("endDate").toDate());
        endDate.setTime(query.value("endTime").toTime());
        event->setEndDate(endDate);

        events.append(event);
    }
    Event *primerEvento = qobject_cast<Event *> (events.first());
    QSqlQuery queryD;
    // We store the time as seconds because it's easier to query.
    queryD.prepare("delete from Event where name = :nombre ");
    queryD.bindValue(":nombre", primerEvento->name());
    queryD.exec();
    events.removeFirst();
    return events;
}


/*
    Defines a helper function to open a connection to an
    in-memory SQLITE database and to create a test table.
*/
void SqlEventModel::createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setHostName("127.0.0.1");
    db.setDatabaseName(":memory:");
//    db.setUserName("root");
//    db.setPassword("root");
    if (!db.open()) {
        qFatal("Cannot open database");
        return;
    }

    QSqlQuery query;
    // We store the time as seconds because it's easier to query.
    query.exec("create table Event (name TEXT, startDate DATE, startTime DATETIME, endDate DATE, endTime DATETIME)");
    query.exec("insert into Event values('Juan Perez', '2016-03-01', '08:00:00', '2016-03-01', '08:30:00')");
    query.exec("insert into Event values('Pedro Salazar', '2016-03-01', '09:00:00', '2016-03-01', '09:30:00')");
    query.exec("insert into Event values('Roberto Rojas', '2016-03-15', '08:00:00', '2016-03-15', '08:30:00')");
    query.exec("insert into Event values('Ana Castro', '2016-03-24', '08:00:00', '2016-03-28', '08:30:00')");

    return;
}
