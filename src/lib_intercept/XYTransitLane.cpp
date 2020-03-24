/*****************************************************************/
/*    NAME: Jason Barker                                      */
/*    ORGN: MIT    */
/*    FILE: XYTransitLane.cpp                                           */
/*    DATE: Mar 23, 2020                                       */
/*****************************************************************/

#include <cstdlib>
#include "XYTransitLane.h"
#include "MBUtils.h"
#include "XYFormatUtilsPoly.h"
#include "XYFormatUtilsSegl.h"
#include "AngleUtils.h"
#include "GeomUtils.h"
#include <numeric>
#include <iostream>
#include <algorithm> 
#include <math.h>


using namespace std;

//-----------------------------------------------------------------
//Procedure: setLaneMidPts

void XYTransitLane::setLaneMidPts(XYPolygon poly)
{
        double x1 = poly.get_vx(0);//assumption: polygons only have four points (weakness)
        double y1 = poly.get_vy(0);//not rubust to more polygons

        double x2 = poly.get_vx(1);
        double y2 = poly.get_vy(1);

        double x3 = poly.get_vx(2);
        double y3 = poly.get_vy(2);        

        double x4 = poly.get_vx(3);
        double y4 = poly.get_vy(3);

        double length1 = distPointToPoint(x1, y1, x2, y2);//determine which side is the longest
        double length2 = distPointToPoint(x1, y1, x4, y4); //assumption that path through is along the longest leg (weakness)

        if(length1>length2){
          //determine midpoints of opposite ends to create a viewable seglist
          double x_mid1 = (x1 + x4)/2;
          double x_mid2 = (x2 + x3)/2;

          double y_mid1 = (y1 + y4)/2;
          double y_mid2 = (y2 + y3)/2;
          m_bisect.add_vertex(x_mid1, y_mid1);
          m_bisect.add_vertex(x_mid2, y_mid2);       
        }
        else{
          double x_mid1 = (x1 + x2)/2;
          double x_mid2 = (x3 + x4)/2;

          double y_mid1 = (y1 + y2)/2;
          double y_mid2 = (y3 + y4)/2;
          m_bisect.add_vertex(x_mid1, y_mid1);
          m_bisect.add_vertex(x_mid2, y_mid2);     
        }	

} 

//-----------------------------------------------------------------
//Procedure: setLaneDirection

void XYTransitLane::setLaneDirection(std::string str)
{
	if((str == "inbound")||(str == "port")){
	  m_transitlane.set_color("label", "red");
      m_transitlane.set_color("vertex", "red");
      m_transitlane.set_color("edge", "red");
      m_transitlane.set_color("fill", "red");
      m_transitlane.set_transparency(0.05);
      m_transitlane.set_edge_size(1);
      m_transitlane.set_vertex_size(2);
	}

	if((str == "outbound")||(str == "starboard")){
	  m_transitlane.set_color("label", "green");
      m_transitlane.set_color("vertex", "green");
      m_transitlane.set_color("edge", "green");
      m_transitlane.set_color("fill", "green");
      m_transitlane.set_transparency(0.025);
      m_transitlane.set_edge_size(1);
      m_transitlane.set_vertex_size(2);		
	}

}

//-----------------------------------------------------------------
//Procedure: setLaneHeading

void XYTransitLane::setLaneHeading(std::string str)
{
	m_heading = stod(str);
}
