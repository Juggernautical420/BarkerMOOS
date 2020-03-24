/*****************************************************************/
/*    NAME: Jason Barker                                      */
/*    ORGN: MIT    */
/*    FILE: XYTransitLane.h                                           */
/*    DATE: Mar 23, 2020                                       */
/*****************************************************************/

#ifndef XY_TRANSIT_LANE_HEADER
#define XY_TRANSIT_LANE_HEADER

#include <string>
#include "XYPolygon.h"
#include "XYSegList.h"

class XYTransitLane
{
 public:
  XYTransitLane();
  virtual ~XYTransitLane() {}

public:
// Setters
void setLanePoly(XYPolygon poly)	{m_transitlane=poly;}
void setLaneMidPts(XYPolygon poly);
void setLaneDirection(std::string str);
void setLaneHeading(std::string str);


// Getters
XYPolygon getLanePoly() const		{return(m_transitlane);}
std::string getLanePolySpecs() const {return(m_transitlane.get_spec());}
XYSegList getLaneMidPts() const		{return(m_bisect);}
std::string getLaneMidPtsSpec() const  {return(m_bisect.get_spec_pts());}
double getLaneHeading() const		{return(m_heading);}


protected: // Key properties
XYPolygon m_transitlane;
XYSegList m_bisect;
std::string m_direction;
double m_heading;


};

#endif   