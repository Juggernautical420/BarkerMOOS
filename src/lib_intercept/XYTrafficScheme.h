/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT - 2N                                             */
/*    FILE: XYTrafficScheme.h                                          */
/*    DATE: Mar 24, 2020                            */
/************************************************************/

#ifndef XY_TRAFFIC_SCHEME_HEADER
#define XY_TRAFFIC_SCHEME_HEADER

#include <iterator>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include "XYObject.h"
#include "XYSegList.h"
#include "XYFormatUtilsSegl.h"
#include "MBUtils.h"
#include "XYPolygon.h"
#include "XYFormatUtilsPoly.h"
#include "AngleUtils.h"
#include "GeomUtils.h"
#include <numeric>
#include <iostream>
#include <algorithm> 

class XYTrafficScheme : public XYObject {
 public:
   XYTrafficScheme() {}
   virtual ~XYTrafficScheme() {}

 public: 
 void addTrafficLane(std::string);	
 unsigned int size() const  {return(m_polyspec.size());}   
 std::string get_lanepolyspec(unsigned int) const;
 std::string get_lanebisect(unsigned int) const;
 std::string get_lanedesignation(unsigned int) const;
 double get_laneheading(unsigned int) const;


 protected: 
 std::vector<std::string> m_polyspec;
 std::vector<std::string> m_polybisect;
 std::vector<std::string> m_designation;
 std::vector<double> m_heading;


 };	

 #endif  