// timenegine.cpp
//
// Task scheduler for gncd(8).
//
//   (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <stdio.h>

#include "db.h"
#include "timeengine.h"

TimeEngine::TimeEngine(QObject *parent)
  : QObject(parent)
{
  engine_pending_guid=0;

  engine_timer=new QTimer(this);
  engine_timer->setSingleShot(true);
  connect(engine_timer,SIGNAL(timeout()),this,SLOT(triggerData()));
}


void TimeEngine::reload()
{
  QString sql;
  SqlQuery *q=NULL;
  QDateTime now=QDateTime::currentDateTime();

  //
  // Check the remainder of today
  //
  sql=QString("select ")+
    "GUID,"+
    "START_TIME "+
    "from EVENTS where "+
    DowSql(now.date())+" and "+
    QString().sprintf("START_TIME>%d ",QTime().secsTo(now.time()))+
    "order by START_TIME "+
    "limit 1";
  q=new SqlQuery(sql);
  if(q->first()) {
    engine_pending_guid=q->value(0).toUInt();
    engine_timer->
      start(now.time().msecsTo(QTime().addSecs(q->value(1).toInt())));
    delete q;
    return;
  }
  delete q;

  //
  // Check the rest of the week
  //
  for(int i=0;i<7;i++) {
    QDate date=now.date().addDays(i+1);
    sql=QString("select ")+
      "GUID,"+
      "START_TIME "+
      "from EVENTS where "+
      DowSql(date)+" "+
      "order by START_TIME "+
      "limit 1";
    q=new SqlQuery(sql);
    if(q->first()) {
      engine_pending_guid=q->value(0).toUInt();
      engine_timer->
	start(now.time().msecsTo(QTime(23,59,59,999))+86400000*i+
	      q->value(1).toInt()*1000);
      //printf("SET GUID: %u  MSECS: %d\n",engine_pending_guid,engine_timer->interval());
      delete q;
      return;
    }
    delete q;
  }
}


void TimeEngine::triggerData()
{
  emit eventTriggered(engine_pending_guid);
  reload();
}


QString TimeEngine::DowSql(const QDate &date)
{
  return QDate::shortDayName(date.dayOfWeek()).toUpper()+"=1";
}
