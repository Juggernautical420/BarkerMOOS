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
#include "XYObject.h"
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
void setupPreCautionAreas();
void setupSeparationZones();
void setupTrafficLanes();
void Concatenate(XYSegList seglist);
void ForceConcatenate(std::vector<std::string>polys, std::vector<std::string> seglists);


// Getters
unsigned int size() const {return(m_traffic_scheme.size());}
std::vector<std::string> getAllViewableObjectSpecs() const;
std::vector<std::string> getGeneratedPolyPts() const;
std::vector<std::string> getGeneratedPolyHdgs() const;
std::vector<std::string> getLaneBoundaries() const;
std::vector<std::string> getSepZonePolys() const;
std::vector<std::string> getSepZonePolyPts() const;
std::vector<std::string> getInboundPolys() const;
std::vector<std::string> getOutboundPolys() const;

void print() const;
void printviewable();


protected:	
std::vector<TrafficObject> m_traffic_scheme;
std::string m_type;
std::string m_polytype;
std::string m_points;
std::string m_name;
std::string m_startpt;
XYPoint m_given_starting_point;


// Viewable Objects //
std::vector<std::string> m_tss_polygons;
std::vector<std::string> m_tss_seglists;

// Concatenate Objects//
double m_init_dist1;
double m_init_dist2;
double m_curr_dist1;
double m_curr_dist2;
double m_x1;
double m_y1;
double m_x2;
double m_y2;
double m_curr_x4;
double m_curr_y4;
double m_curr_x3;
double m_curr_y3;
double m_final_x4;
double m_final_y4;
double m_final_x3;
double m_final_y3;

std::vector<XYPolygon> m_sep_zones;
std::vector<std::string> m_poly_headings;
std::vector<std::string> m_lane_polys;
std::vector<std::string> m_inbound_lanes;
std::vector<std::string> m_outbound_lanes;


};

#endif