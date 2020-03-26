/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT - 2N                                             */
/*    FILE: XYTrafficScheme.cpp                                          */
/*    DATE: Mar 24, 2020                            */
/************************************************************/

#include "XYTrafficScheme.h"

using namespace std;


//---------------------------------------------------------------
// Procedure: addTrafficObject

void XYTrafficScheme::addTrafficObject(const XYTrafficObject trafficobj)
{
  m_traffic_scheme.push_back(trafficobj);
}

//---------------------------------------------------------------
// Procedure: get_TrafficObject

XYTrafficObject XYTrafficScheme::getTrafficObject(unsigned int index) const
{
  if(index>=m_traffic_scheme.size()){
    XYTrafficObject null_object;
     return(null_object);
  }
  
    return(m_traffic_scheme[index]);
}