/*
 * Router.h
 *
 *  Created on: 16 сент. 2017 г.
 *      Author: kived
 */

#ifndef ROUTER_H_
#define ROUTER_H_


#include "Common.h"
#include "Map.h"

class IRouter
{
public:
  virtual bool Create(Map & m) = 0;
  virtual bool Next() = 0;
  virtual RouteItem getRouteItem() = 0;
  virtual ~IRouter(){};
};



#endif /* ROUTER_H_ */
