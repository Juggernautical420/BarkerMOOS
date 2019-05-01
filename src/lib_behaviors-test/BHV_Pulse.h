/************************************************************/
/*    NAME: Jason Barker                                    */
/*    ORGN: MIT                                             */
/*    FILE: BHV_Pulse.h                                     */
/*    DATE: 18 Apr 2019                                     */
/************************************************************/

#ifndef Pulse_HEADER
#define Pulse_HEADER

#include <string>
#include "IvPBehavior.h"
#include "XYRangePulse.h"

class BHV_Pulse : public IvPBehavior {
public:
  BHV_Pulse(IvPDomain);
  ~BHV_Pulse() {};
  
  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();

protected: // Local Utility functions


protected: // Configuration parameters

protected: // State variables
double 	m_current_x;
double 	m_current_y;
double 	m_moos_time;
double  m_current_wpt;
double 	m_next_wpt;
double 	m_wpt_time;
double 	m_range;
double	m_pulse_duration;

XYRangePulse pulse;
};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_Pulse(domain);}
}
#endif
