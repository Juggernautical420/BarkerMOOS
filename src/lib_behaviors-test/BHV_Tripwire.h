/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_Tripwire.h                                      */
/*    DATE:                                                 */
/************************************************************/

#ifndef Tripwire_HEADER
#define Tripwire_HEADER

#include <string>
#include "IvPBehavior.h"
#include "XYRangePulse.h"
#include "XYPoint.h"
#include "XYPolygon.h"
#include "XYSegList.h"



class BHV_Tripwire : public IvPBehavior {
public:
  BHV_Tripwire(IvPDomain);
  ~BHV_Tripwire() {};
  
  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();
  
  bool got_heading;
  bool tripwire_active;
 
  double  zig_direction;
  bool getting_hot;
  bool getting_cold;

 
  bool cycle_done;
  double m_left_tave;
  double m_right_tave;

protected: // Local Utility functions
IvPFunction* buildFunctionWithZAIC();
void      handleVisualHint(std::string);
void      postViewablePolygon();
void      postErasablePolygon();
//void      TempPoints(std::string);

protected: // Configuration parameters
vector<string> myvector;
std::string  m_descriptor;
std::vector<XYPolygon> m_polygons;
XYPolygon m_polygon;
std::string m_hint_vertex_color;
std::string m_hint_edge_color;
double      m_hint_vertex_size;
double      m_hint_edge_size;


protected: // State variables
double  m_current_x;
double  m_current_y;
double  m_current_heading;
double  m_tripwire_heading;
double  m_distance;

double m_current_temp;
double m_low_temp;
double m_high_temp;
double m_delta_temp;
double m_low_temp_turn;
double m_high_temp_turn;
double m_avg_temp;

std::string m_cycle_done;

double  m_current_wpt;
double  m_next_wpt;

double  m_zig_angle;



double m_current_time;

};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_Tripwire(domain);}
}
#endif
