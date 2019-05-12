/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_DetailSurvey.h                                      */
/*    DATE:                                                 */
/************************************************************/

#ifndef DetailSurvey_HEADER
#define DetailSurvey_HEADER

#include <string>
#include "IvPBehavior.h"
#include "XYRangePulse.h"
#include "XYPoint.h"
#include "XYPolygon.h"
#include "XYSegList.h"

class BHV_DetailSurvey : public IvPBehavior {
public:
  BHV_DetailSurvey(IvPDomain);
  ~BHV_DetailSurvey() {};
  
  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();

  double  zig_direction;
  bool getting_hot;
  bool getting_cold;

 bool cycle_done;
  double m_left_tave;
  double m_right_tave;

protected: // Local Utility functions
IvPFunction* buildFunctionWithZAIC();

protected: // Configuration parameters
double m_current_temp;
double m_low_temp;
double m_high_temp;
double m_delta_temp;
double m_low_temp_turn;
double m_high_temp_turn;
double m_avg_temp;

std::string m_cycle_done;

double  m_current_x;
double  m_current_y;
double  m_current_heading;




protected: // State variables
};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_DetailSurvey(domain);}
}
#endif
