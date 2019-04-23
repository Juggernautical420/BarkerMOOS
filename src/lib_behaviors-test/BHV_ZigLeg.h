/************************************************************/
/*    NAME: Jason Barker                                    */
/*    ORGN: MIT                                             */
/*    FILE: BHV_ZigLeg.h                                     */
/*    DATE: 18 Apr 2019                                     */
/************************************************************/

#ifndef ZigLeg_HEADER
#define ZigLeg_HEADER

#include <string>
#include "IvPBehavior.h"
#include "XYRangePulse.h"

class BHV_ZigLeg : public IvPBehavior {
public:
  BHV_ZigLeg(IvPDomain);
  ~BHV_ZigLeg() {};
  
  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();
  double  m_moos_time;

protected: // Local Utility functions
IvPFunction* buildFunctionWithZAIC();



protected: // Configuration parameters

protected: // State variables
double 	m_current_x;
double 	m_current_y;
double  m_current_heading;

double  m_current_time;
double  m_current_wpt;
double 	m_next_wpt;
double 	m_wpt_time;
double  m_end_time;
double 	m_zig_angle;
double	m_zig_duration;
};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_ZigLeg(domain);}
}
#endif
