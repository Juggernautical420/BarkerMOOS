/*****************************************************************/
/*    NAME: Jason Barker                                      */
/*    ORGN: MIT   												 */
/*    FILE: TrafficScheme.h                                           */
/*    DATE: Mar 30, 2020                                       */
/*****************************************************************/

#ifndef TRAFFIC_SCHEME_HEADER
#define TRAFFIC_SCHEME_HEADER

#include <string>
#include <vector>
#include "TrafficObject.h"

class TrafficScheme
{
 public:
  TrafficScheme();
  virtual ~TrafficScheme() {}

// Setters 
bool addTrafficObject(TrafficObject trafficobject);

// Getters
unsigned int size() const {return(m_traffic_scheme.size());}

void print() const;


protected:
std::vector<TrafficObject> m_traffic_scheme;

};

#endif