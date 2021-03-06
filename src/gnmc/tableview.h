// tableview.h
//
// Table Viewer for GlassNet
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

#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <map>

#include <QTableView>

class TableView : public QTableView
{
  Q_OBJECT
 public:
  TableView(QWidget *parent=0);
  bool select(int column,unsigned value);
  bool select(int column,const QString &value,bool case_sensitive=true);
};


#endif  //  TABLEVIEW_H
