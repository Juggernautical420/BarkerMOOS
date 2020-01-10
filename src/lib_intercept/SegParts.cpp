/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT - 2N                                             */
/*    FILE: SegParts.cpp                                          */
/*    DATE: January 7, 2020                            */
/************************************************************/

#include "SegParts.h"
#include <math.h> 

using namespace std;

//-----------------------------------------------------------------
//Procedure: getParts
void SegParts::getParts(XYSegList os_seglist, double m_navSpd)
{
int i;
for(i=1; i<os_seglist.size(); i++){
  double m_x1 = os_seglist.get_vx(i-1);
  double m_y1 = os_seglist.get_vy(i-1);
  double m_x2 = os_seglist.get_vx(i);
  double m_y2 = os_seglist.get_vy(i);

  double m_heading = 90 - (atan2 ((m_y2 - m_y1),(m_x2 - m_x1))) * 180/M_PI;
  m_legHeading.push_back(m_heading);

  double m_distance = sqrt(pow((m_x2 - m_x1), 2) + pow((m_y2 - m_y1), 2));
  m_legLength.push_back(m_distance);

  double timeOnLeg = m_distance/m_navSpd;
  m_legTime.push_back(timeOnLeg);
 }
} 

//---------------------------------------------------------------
// Procedure: get_legHeading

double SegParts::get_legHeading(unsigned int i) const
{
  if(i<m_legHeading.size())
     return(m_legHeading[i]);
  else
    return(0);
}

//---------------------------------------------------------------
// Procedure: get_legLength

double SegParts::get_legLength(unsigned int i) const
{
  if(i<m_legHeading.size())
     return(m_legLength[i]);
  else
    return(0);
}

//---------------------------------------------------------------
// Procedure: get_legTime
// Does not work in current form

double SegParts::get_legTime(unsigned int i) const
{
  if(i<m_legHeading.size())
     return(m_legTime[i]);
  else
    return(0);
}

