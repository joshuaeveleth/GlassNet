// mysql.cpp
//
// MySQL Database routines for gnmc(1).
//
// (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
//     All Rights Reserved.
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

#include "crypto.h"
#include "db.h"
#include "gnmc.h"

bool MainWidget::CheckDb(QString *err_msg)
{
  QString sql=QString("select DB from VERSION");
  SqlQuery *q=new SqlQuery(sql);
  if(!q->first()) {
    if(!CreateDb()) {
      *err_msg=tr("unable to create database");
      return false;
    }
  }
  delete q;
  if(!CheckSchema()) {
    *err_msg=tr("invalid/unrecognized database schema");
    return false;
  }

  return true;
}


bool MainWidget::CreateDb()
{
  QString sql;
  bool ok=false;

  sql=QString("create table VERSION(DB int not null)")+
    gnmc_config->createTablePostfix();
  SqlQuery::run(sql,&ok);
  if(!ok) {
    return false;
  }
  sql=QString("insert into VERSION values(0)");
  SqlQuery::run(sql,&ok);

  return ok;
}


bool MainWidget::CheckSchema()
{
  int schema;
  bool ok=false;
  QString sql;
  SqlQuery *q=NULL;

  sql=QString("select DB from VERSION");
  q=new SqlQuery(sql);
  if(!q->first()) {
    delete q;
    return false;
  }
  schema=q->value(0).toInt();
  delete  q;

  if(schema<1) {
    sql=QString("create table USERS (")+
      "ID integer primary key auto_increment,"+
      "USERNAME char(255) unique not null,"+
      "PASSWORD char(44) not null,"+
      "FULL_NAME text,"+
      "DESCRIPTION text,"+
      "USER_PRIV int not null default 0,"+
      "SITE_PRIV int not null default 0,"+
      "EVENT_PRIV int not null default 0)";
      gnmc_config->createTablePostfix();
    SqlQuery::run(sql,&ok);
    if(!ok) {
      return false;
    }

    sql=QString("insert into USERS set ")+
      "USERNAME='"+SqlQuery::escape("admin")+"',"+
      "FULL_NAME='"+SqlQuery::escape(tr("GlassNet Administrator"))+"',"+
      "PASSWORD='"+SqlQuery::escape(MakePasswordHash(""))+"',"+
      "USER_PRIV=1,"+
      "SITE_PRIV=1,"+
      "EVENT_PRIV=1";
    SqlQuery::run(sql,&ok);
    if(!ok) {
      return false;
    }

    sql=QString("create table SITES (")+
      "ID integer primary key auto_increment,"+
      "NAME char(255) not null,"+
      "REMARKS text,"+
      "index NAME_IDX(NAME))"+
      gnmc_config->createTablePostfix();
    SqlQuery::run(sql,&ok);
    if(!ok) {
      return false;
    }
  }

  if(schema<2) {
    sql=QString("create table CHASSIS (")+
      "ID integer primary key auto_increment,"+
      "SITE_ID int default null,"+
      "SLOT int default null,"+
      "TYPE int not null,"+
      "SERIAL_NUMBER char(32) not null,"+
      "index SITE_ID_IDX(SITE_ID,SLOT))";
    SqlQuery::run(sql,&ok);
    if(!ok) {
      return false;
    }
  }

  if(schema<3) {
    sql=QString("create table RECEIVERS (")+
      "ID integer primary key auto_increment,"+
      "CHASSIS_ID int default null,"+
      "SLOT int default null,"+
      "TYPE int not null,"+
      "MAC_ADDRESS char(18) unique not null,"+
      "index CHASSIS_ID_IDX(CHASSIS_ID,SLOT))";
    SqlQuery::run(sql,&ok);
    if(!ok) {
      return false;
    }
  }


  //
  // *** End of schema updates ***
  //

  sql=QString("update VERSION set ")+
    QString().sprintf("DB=%d",GLASSNET_SCHEMA_VERSION);
  SqlQuery::run(sql,&ok);

  return ok;
}