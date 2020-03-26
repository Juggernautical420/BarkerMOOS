/*****************************************************************/
/*    NAME: Jason Barker                                      */
/*    ORGN: MIT    */
/*    FILE: XYTransitLane.h                                           */
/*    DATE: Mar 23, 2020                                       */
/*****************************************************************/

#ifndef XY_TRANSIT_LANE_HEADER
#define XY_TRANSIT_LANE_HEADER

#include <string>
#include <vector>
#include "XYPolygon.h"
#include "XYSegList.h"
#include "XYPoint.h"
#include "XYFormatUtilsPoly.h"
#include "XYFormatUtilsSegl.h"

class XYTransitLane : public XYPolygon{
public:
 XYTransitLane();
 virtual ~XYTransitLane() {}

void setLanePoly(XYPolygon poly);

public:





protected: // Key properties
XYPolygon m_poly;
std::string m_label;

XYSegList m_enter;
XYSegList m_exit;

XYPoint m_enter_midpt;
XYPoint m_exit_midt;

double m_poly_heading;
double m_poly_radius;

private:



};

#endif   