/*****************************************************************/
/*    NAME: M.Benjamin, H.Schmidt, J. Leonard                    */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: BHV_SimpleWaypoint.ch                                */
/*    DATE: July 1st 2008  (For purposes of simple illustration) */
/*    MODIFIED: Jason Barker                                     */
/*    FILE: BHV_InitialSweep.h                                   */
/*    DATE: May 5, 2019                                          */
/*                                                               */
/* This program is free software; you can redistribute it and/or */
/* modify it under the terms of the GNU General Public License   */
/* as published by the Free Software Foundation; either version  */
/* 2 of the License, or (at your option) any later version.      */
/*                                                               */
/* This program is distributed in the hope that it will be       */
/* useful, but WITHOUT ANY WARRANTY; without even the implied    */
/* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR       */
/* PURPOSE. See the GNU General Public License for more details. */
/*                                                               */
/* You should have received a copy of the GNU General Public     */
/* License along with this program; if not, write to the Free    */
/* Software Foundation, Inc., 59 Temple Place - Suite 330,       */
/* Boston, MA 02111-1307, USA.                                   */
/*****************************************************************/
 
#ifndef BHV_INITIAL_SWEEP_HEADER
#define BHV_INITIAL_SWEEP_HEADER

#include <string>
#include "IvPBehavior.h"
#include "XYPoint.h"
#include "XYPolygon.h"

class BHV_InitialSweep : public IvPBehavior {
public:
  BHV_InitialSweep(IvPDomain);
  ~BHV_InitialSweep() {};
  
  bool         setParam(std::string, std::string);
  void         onIdleState();
  IvPFunction* onRunState();
  bool got_heading;

protected:
  void         postViewPoint(bool viewable=true);
  IvPFunction* buildFunctionWithZAIC();
  IvPFunction* buildTripwireWithZAIC();
  IvPFunction* buildFunctionWithReflector();
  void      handleVisualHint(std::string);
  void      postViewablePolygon();
  void      postErasablePolygon();
  double  m_moos_time;  

protected: // Configuration parameters
  double       m_arrival_radius;
  double       m_desired_speed;
  XYPoint      m_nextpt;
  std::string  m_ipf_type;
  std::string  m_descriptor;
  std::vector<XYPolygon> m_polygons;
  XYPolygon m_polygon;
  std::string m_hint_vertex_color;
  std::string m_hint_edge_color;
  double      m_hint_vertex_size;
  double      m_hint_edge_size;

protected: // State variables
  double   m_osx;
  double   m_osy;
  double   m_current_heading;
  double m_distance;


  double  m_current_time;
  double  m_current_wpt;
  double  m_next_wpt;
  double  m_wpt_time;
  double  m_end_time;
  double  m_zig_angle;
  double  m_zig_duration;

};

#ifdef WIN32
	// Windows needs to explicitly specify functions to export from a dll
   #define IVP_EXPORT_FUNCTION __declspec(dllexport) 
#else
   #define IVP_EXPORT_FUNCTION
#endif

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_InitialSweep(domain);}
}
#endif









