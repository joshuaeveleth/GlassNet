// receiver.h
//
// Abstract a GlassNet receiver.
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

#ifndef RECEIVER_H
#define RECEIVER_H

#include <stdint.h>

#include <QString>

#include "accessor.h"
#include "db.h"

class Receiver : public Accessor
{
 public:
  enum Type {TypeNone=0,TypeRaspPi2=1,TypeRaspPi3=2,TypeLast=3};
  Receiver(int id);
  Receiver(int chassis_id,int slot);
  Receiver(const QString &mac);
  int id() const;
  bool exists() const;
  int chassisId() const;
  void setChassisId(int id) const;
  int slot() const;
  void setSlot(int slot) const;
  Type type() const;
  void setType(Type type) const;
  QString macAddress() const;
  void setMacAddress(const QString &mac) const;
  QString description() const;
  static QString typeString(Type type);
  static int create(Type type,const QString &mac);
  static void remove(int receiver_id);
  static bool exists(int receiver_id);

 private:
  QString tableName() const;
  QString whereClause() const;
  int receiver_id;
};


#endif  // RECEIVER_H