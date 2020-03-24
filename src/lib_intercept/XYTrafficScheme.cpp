/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT - 2N                                             */
/*    FILE: XYTrafficScheme.cpp                                          */
/*    DATE: Mar 24, 2020                            */
/************************************************************/

#include "XYTrafficScheme.h"

using namespace std;

//---------------------------------------------------------------
// Procedure: addTrafficLane
// Format of lane= pts={10,-10: 15,-10: 15,30: 10,30}; designation=inbound; hdg=180

void XYTrafficScheme::addTrafficLane(std::string)
{

}



//---------------------------------------------------------------
// Procedure: get_lanepolyspec

string XYTrafficScheme::get_lanepolyspec(unsigned int i) const
{
  if(i<m_polyspec.size())
    return(m_polyspec[i]);
  else
    return("");
}

//---------------------------------------------------------------
// Procedure: get_lanebisect

string XYTrafficScheme::get_lanebisect(unsigned int i) const
{
  if(i<m_polybisect.size())
    return(m_polybisect[i]);
  else
    return("");
}

//---------------------------------------------------------------
// Procedure: get_lanedesignation

string XYTrafficScheme::get_lanedesignation(unsigned int i) const
{
  if(i<m_designation.size())
    return(m_designation[i]);
  else
    return("");
}

//---------------------------------------------------------------
// Procedure: get_laneheading

double XYTrafficScheme::get_laneheading(unsigned int i) const
{
  if(i<m_heading.size())
     return(m_heading[i]);
  else
    return(0);
}