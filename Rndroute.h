/*
 * Rndroute.h
 *
 *  Created on: 16 сент. 2017 г.
 *      Author: kived
 */

#ifndef RNDROUTE_H_
#define RNDROUTE_H_

#include "Router.h"
#include "Direction.h"
#include "Common.h"

class Map;

class RandomRouter : public IRouter
{
public:
  RandomRouter( int ax, int ay,int d)
    :pmap(0),x(ax),y(ay),dir(d){}
  virtual bool Create(Map& m);
  virtual bool Next();
  virtual RouteItem getRouteItem()
  {
    return ri;
  }
private:
  Map * pmap;
  int x;
  int y;
  direction dir;
  RouteItem ri;
};



#endif /* RNDROUTE_H_ */
