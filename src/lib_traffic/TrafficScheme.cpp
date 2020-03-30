/*****************************************************************/
/*    NAME: Jason Barker                                      */
/*    ORGN: MIT   												 */
/*    FILE: TrafficScheme.cpp                                           */
/*    DATE: Mar 30, 2020                                       */
/*****************************************************************/

#include <algorithm>
#include <iostream>

#include "TrafficScheme.h"
#include "MBUtils.h"

using namespace std;

//-----------------------------------------------------------
// Constructor

TrafficScheme::TrafficScheme()
{
}

//-----------------------------------------------------------
// Procedure: addTrafficObject

bool TrafficScheme::addTrafficObject(TrafficObject new_traf_obj)
{

  m_traffic_scheme.push_back(new_traf_obj);
  return(true);
}


//-----------------------------------------------------------
// Procedure: print()

void TrafficScheme::print() const
{
  cout << "Total Traffic Objects: " << m_traffic_scheme.size() << endl;
  for(unsigned int i=0; i<m_traffic_scheme.size(); i++) {
    cout << "==================================================" << endl;
    m_traffic_scheme[i].print();
  }    
}