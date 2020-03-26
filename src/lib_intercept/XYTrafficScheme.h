/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT - 2N                                             */
/*    FILE: XYTrafficScheme.h                                          */
/*    DATE: Mar 24, 2020                            */
/************************************************************/

#ifndef XY_TRAFFIC_SCHEME_HEADER
#define XY_TRAFFIC_SCHEME_HEADER

#include <iterator>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include "MBUtils.h"
#include "GeomUtils.h"
#include "XYPolygon.h"
#include "XYTrafficObject.h"



class XYTrafficScheme   
{
 public:
   XYTrafficScheme() {}
   virtual ~XYTrafficScheme() {}

 public:  
 void addTrafficObject(const XYTrafficObject trafficobj);

 XYTrafficObject getTrafficObject(unsigned int) const;	

 unsigned int size() const 		{return(m_traffic_scheme.size());}

 protected: 
 std::vector<XYTrafficObject> m_traffic_scheme;
 


 };	

 #endif  