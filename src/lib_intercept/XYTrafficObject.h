/*****************************************************************/
/*    NAME: Jason Barker                                      */
/*    ORGN: MIT   												 */
/*    FILE: XYTrafficObject.h                                           */
/*    DATE: Mar 26, 2020                                       */
/*****************************************************************/

#ifndef XY_TRAFFIC_OBJECT_HEADER
#define XY_TRAFFIC_OBJECT_HEADER

#include <string>
#include <vector>
#include "XYPolygon.h"
#include "XYSegList.h"
#include "XYPoint.h"
#include "XYFormatUtilsPoly.h"
#include "XYFormatUtilsSegl.h"

class XYTrafficObject : public XYPolygon{
public:
 XYTrafficObject();
 virtual ~XYTrafficObject() {}

public:
void createTrafficPoly(std::string str)			{m_poly = string2Poly(str);}	
void setTrafficPoly(XYPolygon poly) 			{m_poly=poly;}
void setTrafficPolyType(std::string str);
void setTrafficDirection(std::string str1, std::string str2);

// Getters
XYPolygon getTrafficPoly() const				{return(m_poly);}
std::string getTrafficPolySpecs() const			{return(m_poly.get_spec());}
XYSegList getTrafficEntrance() const			{return(m_enter);}
std::string getTrafficEntranceSpecs() const		{return(m_enter_spec);}
XYSegList getTrafficExit() const				{return(m_exit);}
std::string getTrafficExitSpecs() const			{return(m_exit_spec);}
double getTrafficHeading() const				{return(m_poly_heading);}
double getPreCautionArea() const				{return(m_poly.max_radius());}
std::string getTrafficPolyType() const			{return(m_label);}



protected: // Key properties	
XYPolygon m_poly;
std::string m_label;


// Derived properties
XYSegList m_enter;
XYSegList m_exit;
std::string m_enter_spec;
std::string m_exit_spec;

XYPoint m_enter_midpt;
XYPoint m_exit_midpt;

double m_poly_heading;

private:



};

#endif  