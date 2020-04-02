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
#include "XYPolygon.h"
#include "XYPoint.h"
#include "XYSegList.h"
#include "XYFormatUtilsPoly.h"
#include "XYFormatUtilsSegl.h"
#include "XYFormatUtilsPoint.h"


class TrafficScheme
{
 public:
  TrafficScheme();
  virtual ~TrafficScheme() {}

// Setters 
bool addTrafficObject(TrafficObject trafficobject);
void setupTrafficLanes();
void Concatenate(XYSegList seglist, std::string tss_type);

// Getters
unsigned int size() const {return(m_traffic_scheme.size());}

void print() const;
void printviewable();


protected:	
std::vector<TrafficObject> m_traffic_scheme;
std::string type;
std::string polytype;
std::string points;
std::string name;
std::string startpt;
XYPoint given_starting_point;


// Viewable Objects //
std::vector<std::string> m_tss_polygons;
std::vector<std::string> m_tss_seglists;

// Concatenate Objects//
double init_dist1;
double init_dist2;
double curr_dist1;
double curr_dist2;
double x1;
double y1;
double x2;
double y2;
double curr_x4;
double curr_y4;
double curr_x3;
double curr_y3;
double final_x4;
double final_y4;
double final_x3;
double final_y3;

std::vector<XYPolygon> m_sep_zones;
std::vector<std::string> m_poly_headings;
std::vector<std::string> m_lane_polys;

};

#endif