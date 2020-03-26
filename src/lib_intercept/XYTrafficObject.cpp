/*****************************************************************/
/*    NAME: Jason Barker                                      */
/*    ORGN: MIT   												 */
/*    FILE: XYTrafficObject.cpp                                           */
/*    DATE: Mar 26, 2020                                       */
/*****************************************************************/

#include <cstdlib>
#include "XYTrafficObject.h"
#include "MBUtils.h"
#include "AngleUtils.h"
#include "GeomUtils.h"
#include <numeric>
#include <iostream>
#include <algorithm> 
#include <math.h>


using namespace std;

//-----------------------------------------------------------
// Procedure: setTrafficPolyType

void XYTrafficObject::setTrafficPolyType(std::string str)
{
	if((str == "inbound")||(str == "inbound lane")){
	  m_poly.set_color("label", "red");
      m_poly.set_color("vertex", "red");
      m_poly.set_color("edge", "red");
      m_poly.set_color("fill", "red");
      m_poly.set_transparency(0.05);
      m_poly.set_edge_size(1);
      m_poly.set_vertex_size(2);

      m_label = str;	
	}

	if((str == "outbound")||(str == "outbound lane")){
	  m_poly.set_color("label", "green");
      m_poly.set_color("vertex", "green");
      m_poly.set_color("edge", "green");
      m_poly.set_color("fill", "green");
      m_poly.set_transparency(0.025);
      m_poly.set_edge_size(1);
      m_poly.set_vertex_size(2);

      m_label = str;		
	}

	if((str == "precaution")||(str == "precaution area")){
	  m_poly.set_color("label", "purple");
      m_poly.set_color("vertex", "purple");
      m_poly.set_color("edge", "purle");
      m_poly.set_color("fill", "purle");
      m_poly.set_transparency(0.05);
      m_poly.set_edge_size(1);
      m_poly.set_vertex_size(2);

      m_label = str;		
	}

	if((str == "separation")||(str == "separation zone")){
	  m_poly.set_color("label", "purle");
      m_poly.set_color("vertex", "purple");
      m_poly.set_color("edge", "purple");
      m_poly.set_color("fill", "purple");
      m_poly.set_transparency(0.025);
      m_poly.set_edge_size(1);
      m_poly.set_vertex_size(2);

      m_label = str;		
	}
}

//-----------------------------------------------------------
// Procedure: setTrafficDirection

void XYTrafficObject::setTrafficDirection(std::string str1, std::string str2)
{
	m_enter = string2SegList(str1);
	m_enter_spec = m_enter.get_spec_pts();

	m_exit = string2SegList(str2);
	m_exit_spec = m_exit.get_spec_pts();

	m_enter_midpt.set_vertex(m_enter.get_center_x(), m_enter.get_center_y());
	m_exit_midpt.set_vertex(m_exit.get_center_x(), m_exit.get_center_y());

	m_poly_heading = relAng(m_enter_midpt, m_exit_midpt);
}