/*****************************************************************/
/*    NAME: Jason Barker                                      */
/*    ORGN: MIT   												 */
/*    FILE: TrafficScheme.cpp                                           */
/*    DATE: Mar 30, 2020                                       */
/*****************************************************************/

#include <algorithm>
#include <iostream>
#include "AngleUtils.h"
#include "GeomUtils.h"
#include "TrafficScheme.h"
#include "MBUtils.h"


using namespace std;

//-----------------------------------------------------------
// Constructor

TrafficScheme::TrafficScheme()
{

}

//-----------------------------------------------------------
// Procedure: addTrafficObject

bool TrafficScheme::addTrafficObject(TrafficObject new_traf_obj)
{
  type = new_traf_obj.getTrafficType();
  polytype = new_traf_obj.getPolyType();
  points = new_traf_obj.getPolyPts();
  name = new_traf_obj.getPolyName();
  startpt = new_traf_obj.getPolyStartPts();

  if((type == "precaution area")||(type == "precaution")){
  	  XYPolygon m_poly;
  	  string radial = "format=radial, " + stripBlankEnds(points) + ", pts=12, snap=1";
  	  m_poly = string2Poly(radial);
	  m_poly.set_color("label", "purle");
      m_poly.set_color("vertex", "purple");
      m_poly.set_color("edge", "purple");
      m_poly.set_color("fill", "purple");
      m_poly.set_transparency(0.025);
      m_poly.set_edge_size(1);
      m_poly.set_vertex_size(2);

      if(name != "")
      m_poly.set_label(name);
  	  string m_poly_specs = m_poly.get_spec();
      m_tss_polygons.push_back(m_poly_specs);	
  }

  if((type == "separation zone")||(type == "separation")){
  	  XYPolygon m_poly;
  	  m_poly = string2Poly(points);
	  m_poly.set_color("label", "purle");
      m_poly.set_color("vertex", "purple");
      m_poly.set_color("edge", "purple");
      m_poly.set_color("fill", "purple");
      m_poly.set_transparency(0.025);
      m_poly.set_edge_size(1);
      m_poly.set_vertex_size(2);

      if(name != "")
      m_poly.set_label(name);
  	  m_sep_zones.push_back(m_poly);
  	  string m_poly_specs = m_poly.get_spec();
      m_tss_polygons.push_back(m_poly_specs);	
  }


  if((type == "inbound lane")||(type == "inbound"))
	setupTrafficLanes();

  if((type == "outbound lane")||(type == "outbound"))
	setupTrafficLanes();



  m_traffic_scheme.push_back(new_traf_obj);
  return(true);
	
}


//-----------------------------------------------------------
// Procedure: print()

void TrafficScheme::print() const
{
  cout << "Total Traffic Objects: " << m_traffic_scheme.size() << endl;
  for(unsigned int i=0; i<m_traffic_scheme.size(); i++) {
    cout << "==================================================" << endl;
    m_traffic_scheme[i].print();

  }    
}

//-----------------------------------------------------------
// Procedure: printviewable()
void TrafficScheme::printviewable()
{
	cout << "Summary of Viewable Objects "<< endl;
	cout << "==================================================" << endl;
	cout << "Total Number Of Polygons: " << m_tss_polygons.size() << endl;
	 for(unsigned int i=0; i<m_tss_polygons.size(); i++) {
     cout << m_tss_polygons[i] << endl;
	 } 
	cout << "==================================================" << endl; 
	 cout << "Total Number of Seglists: " << m_tss_seglists.size() << endl;
	 for(unsigned int i=0; i<m_tss_seglists.size(); i++) {
     cout <<	m_tss_seglists[i] << endl;
    }
	cout << "==================================================" << endl; 
	 cout << "Generated Traffic Lane Polys: " << m_lane_polys.size() << endl;
	 for(unsigned int i=0; i<m_lane_polys.size(); i++) {
     cout <<	m_lane_polys[i] << ",  Heading= " << m_poly_headings[i] << endl;
    }    
}


//-----------------------------------------------------------
// Procedure: setupTrafficLanes
void TrafficScheme::setupTrafficLanes()
{
	XYSegList m_seglist;
	m_seglist =  string2SegList(points);

	if(name != "")
		m_seglist.set_label(name);
	else
		m_seglist.set_label(type);


	given_starting_point = string2Point(startpt);
	double m_x = given_starting_point.get_vx();
	double m_y = given_starting_point.get_vy();

	double m_x_first = m_seglist.get_vx(0);
	double m_y_first = m_seglist.get_vy(0);


	double m_x_last = m_seglist.get_vx(m_seglist.size()-1);
	double m_y_last = m_seglist.get_vy(m_seglist.size()-1);
	

	if((m_x == m_x_first)&&(m_y == m_y_first)){
		Concatenate(m_seglist, type);
		string m_segl_spec = m_seglist.get_spec();
		m_tss_seglists.push_back(m_segl_spec);
  	}

  	if((m_x == m_x_last)&&(m_y == m_y_last)){
		m_seglist.reverse();
		XYSegList new_segl = m_seglist;
		Concatenate(new_segl, type);
		string m_segl_spec = new_segl.get_spec();
		m_tss_seglists.push_back(m_segl_spec);	
  	}
}

//-----------------------------------------------------------
// Procedure: Concatenate
void TrafficScheme::Concatenate(XYSegList seglist, string tss_type)
{
	int lane_objs = seglist.size()-1;
	for(int i=0; i<lane_objs; i++){
			init_dist1 = 1000;
			init_dist2 = 1000;
			x1 = seglist.get_vx(i);
			y1 = seglist.get_vy(i);

			x2 = seglist.get_vx(i+1);
			y2 = seglist.get_vy(i+1);
			string heading = doubleToString(relAng(x1,y1,x2,y2));
			m_poly_headings.push_back(heading);

		for(int j=0; j<m_sep_zones.size(); j++){
			XYSegList curr_poly1 = m_sep_zones[j].exportSegList(x1,y1);
			curr_x4 = curr_poly1.get_vx(0);
			curr_y4 = curr_poly1.get_vy(0);
			curr_dist1 = distPointToPoint(x1,y1,curr_x4,curr_y4);

			XYSegList curr_poly2 = m_sep_zones[j].exportSegList(x2,y2);
			curr_x3 = curr_poly2.get_vx(0);
			curr_y3 = curr_poly2.get_vy(0);
			curr_dist2 = distPointToPoint(x2,y2,curr_x3,curr_y3);
		
			if(curr_dist1 < init_dist1){
				final_x4 = curr_x4;
				final_y4 = curr_y4;			
				init_dist1 = curr_dist1;
			}

			if(curr_dist2 < init_dist2){
				final_x3 = curr_x3;
				final_y3 = curr_y3;				
				init_dist2 = curr_dist2;
			}
		}

		XYPolygon new_poly;
		new_poly.add_vertex(x1,y1);
		new_poly.add_vertex(x2,y2);		
		new_poly.add_vertex(final_x3,final_y3);
		new_poly.add_vertex(final_x4,final_y4);
		string new_poly_pts = new_poly.get_spec();
		m_lane_polys.push_back(new_poly_pts);

		if((tss_type == "inbound lane")||(tss_type == "inbound")){
			new_poly.set_color("label", "red");
      		new_poly.set_color("vertex", "red");
     		new_poly.set_color("edge", "red");
      		new_poly.set_color("fill", "red");
      		new_poly.set_transparency(0.05);
      		new_poly.set_edge_size(1);
      		new_poly.set_vertex_size(2);	
		}

		if((tss_type == "outbound lane")||(tss_type == "outbound")){
		    new_poly.set_color("label", "green");
      	    new_poly.set_color("vertex", "green");
     	    new_poly.set_color("edge", "green");
     	    new_poly.set_color("fill", "green");
     	    new_poly.set_transparency(0.025);
     	    new_poly.set_edge_size(1);
    	    new_poly.set_vertex_size(2);
		}

		string new_specs = new_poly.get_spec();
		m_tss_polygons.push_back(new_specs);

	}
}