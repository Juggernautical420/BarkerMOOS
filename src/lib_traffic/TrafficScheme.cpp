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
  m_type = new_traf_obj.getTrafficType();
  m_polytype = new_traf_obj.getPolyType();
  m_points = new_traf_obj.getPolyPts();
  m_name = new_traf_obj.getPolyName();
  m_startpt = new_traf_obj.getPolyStartPts();

  if((m_type == "precaution area")||(m_type == "precaution"))
  	setupPreCautionAreas();

  if((m_type == "separation zone")||(m_type == "separation"))
  	setupSeparationZones();

  if((m_type == "inbound lane")||(m_type == "inbound"))
	setupTrafficLanes();

  if((m_type == "outbound lane")||(m_type == "outbound"))
	setupTrafficLanes();

  m_traffic_scheme.push_back(new_traf_obj);
  return(true);
	
}

//-----------------------------------------------------------
// Procedure: getAllViewableObjectSpecs()

vector<string> TrafficScheme::getAllViewableObjectSpecs() const
{
	vector<string> rvector;
	for(int i=0; i<m_tss_polygons.size(); i++){
		string tss_poly = m_tss_polygons[i];
		rvector.push_back(tss_poly);
	}

	return(rvector);
}

//-----------------------------------------------------------
// Procedure: getGeneratedPolyPts()

vector<string> TrafficScheme::getGeneratedPolyPts() const
{
	vector<string> rvector;
	for(int i=0; i<m_lane_polys.size(); i++){
		string tss_poly = m_lane_polys[i];
		rvector.push_back(tss_poly);
	}

	return(rvector);
}

//-----------------------------------------------------------
// Procedure: getGeneratedPolyHdgs()

vector<string> TrafficScheme::getGeneratedPolyHdgs() const
{
	vector<string> rvector;
	for(int i=0; i<m_poly_headings.size(); i++){
		string tss_poly = m_poly_headings[i];
		rvector.push_back(tss_poly);
	}

	return(rvector);
}

//-----------------------------------------------------------
// Procedure: getLaneBoundaries()

vector<string> TrafficScheme::getLaneBoundaries() const
{
	vector<string> rvector;
	for(int i=0; i<m_tss_seglists.size(); i++){
		string tss_segs = m_tss_seglists[i];
		rvector.push_back(tss_segs);
	}

	return(rvector);	
}

//-----------------------------------------------------------
// Procedure: getSepZonePolys()

vector<string> TrafficScheme::getSepZonePolys() const
{
	vector<string> rvector;
	for(int i=0; i<m_sep_zones.size(); i++){
		string tss_sepz = m_sep_zones[i].get_spec();
		rvector.push_back(tss_sepz);
	}

	return(rvector);
}

//-----------------------------------------------------------
// Procedure: getSepZonePolyPts()

vector<string> TrafficScheme::getSepZonePolyPts() const
{
	vector<string> rvector;
	for(int i=0; i<m_sep_zones.size(); i++){
		string tss_sepz = m_sep_zones[i].get_spec_pts();
		rvector.push_back(tss_sepz);
	}

	return(rvector);
}

//-----------------------------------------------------------
// Procedure: getInboundPolys()

vector<string> TrafficScheme::getInboundPolys() const
{
	vector<string> rvector;
	for(int i=0; i<m_inbound_lanes.size(); i++){
		string inbound = m_inbound_lanes[i];
		rvector.push_back(inbound);
	}

	return(rvector);
}

//-----------------------------------------------------------
// Procedure: getOutboundPolys()

vector<string> TrafficScheme::getOutboundPolys() const
{
	vector<string> rvector;
	for(int i=0; i<m_outbound_lanes.size(); i++){
		string outbound = m_outbound_lanes[i];
		rvector.push_back(outbound);
	}

	return(rvector);
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
// Procedure: setupPreCautionAreas()
void TrafficScheme::setupPreCautionAreas()
{
  	XYPolygon poly;
  	string radial = "format=radial, " + stripBlankEnds(m_points) + ", pts=20, snap=1";
  	poly = string2Poly(radial);
    poly.set_color("vertex", "purple");
    poly.set_color("edge", "purple");
    poly.set_color("fill", "purple");
    poly.set_transparency(0.025);
    poly.set_edge_size(1);
    poly.set_vertex_size(2);

    if(m_name != "")
    poly.set_label(m_name);
  	string poly_specs = poly.get_spec();
    m_tss_polygons.push_back(poly_specs);	
}

//-----------------------------------------------------------
// Procedure: setupSeparationZones
void TrafficScheme::setupSeparationZones()
{
  	XYPolygon poly;
  	poly = string2Poly(m_points);
    poly.set_color("vertex", "purple");
    poly.set_color("edge", "purple");
    poly.set_color("fill", "purple");
    // poly.set_transparency(0.025);
    poly.set_edge_size(1);
    poly.set_vertex_size(2);

    if(m_name != "")
    poly.set_label(m_name);
  	m_sep_zones.push_back(poly);
  	string poly_specs = poly.get_spec();
    m_tss_polygons.push_back(poly_specs);		
}



//-----------------------------------------------------------
// Procedure: setupTrafficLanes
void TrafficScheme::setupTrafficLanes()
{
	XYSegList seglist;
	seglist =  string2SegList(m_points);

	if(m_name != "")
		seglist.set_label(m_name);
	else
		seglist.set_label(m_type);


	m_given_starting_point = string2Point(m_startpt);
	double x = m_given_starting_point.get_vx();
	double y = m_given_starting_point.get_vy();

	double x_first = seglist.get_vx(0);
	double y_first = seglist.get_vy(0);


	double x_last = seglist.get_vx(seglist.size()-1);
	double y_last = seglist.get_vy(seglist.size()-1);
	

	if((x == x_first)&&(y == y_first)){
		Concatenate(seglist);
		string segl_spec = seglist.get_spec();
		m_tss_seglists.push_back(segl_spec);
  	}

  	if((x == x_last)&&(y == y_last)){
		seglist.reverse();
		XYSegList new_segl = seglist;
		Concatenate(new_segl);
		string segl_spec = new_segl.get_spec();
		m_tss_seglists.push_back(segl_spec);	
  	}
}

//-----------------------------------------------------------
// Procedure: Concatenate
void TrafficScheme::Concatenate(XYSegList seglist)
{
	if(m_sep_zones.size() == 0)
		return;
	int lane_objs = seglist.size()-1;
	for(int i=0; i<lane_objs; i++){
			m_init_dist1 = 1000;
			m_init_dist2 = 1000;
			m_x1 = seglist.get_vx(i);
			m_y1 = seglist.get_vy(i);

			m_x2 = seglist.get_vx(i+1);
			m_y2 = seglist.get_vy(i+1);
			string heading = doubleToString(relAng(m_x1,m_y1,m_x2,m_y2));
			m_poly_headings.push_back(heading);

		for(int j=0; j<m_sep_zones.size(); j++){
			XYSegList curr_poly1 = m_sep_zones[j].exportSegList(m_x1,m_y1);
			m_curr_x4 = curr_poly1.get_vx(0);
			m_curr_y4 = curr_poly1.get_vy(0);
			m_curr_dist1 = distPointToPoint(m_x1,m_y1,m_curr_x4,m_curr_y4);

			XYSegList curr_poly2 = m_sep_zones[j].exportSegList(m_x2,m_y2);
			m_curr_x3 = curr_poly2.get_vx(0);
			m_curr_y3 = curr_poly2.get_vy(0);
			m_curr_dist2 = distPointToPoint(m_x2,m_y2,m_curr_x3,m_curr_y3);
		
			if(m_curr_dist1 < m_init_dist1){
				m_final_x4 = m_curr_x4;
				m_final_y4 = m_curr_y4;			
				m_init_dist1 = m_curr_dist1;
			}

			if(m_curr_dist2 < m_init_dist2){
				m_final_x3 = m_curr_x3;
				m_final_y3 = m_curr_y3;				
				m_init_dist2 = m_curr_dist2;
			}
		}

		XYPolygon new_poly;
		new_poly.add_vertex(m_x1,m_y1);
		new_poly.add_vertex(m_x2,m_y2);		
		new_poly.add_vertex(m_final_x3,m_final_y3);
		new_poly.add_vertex(m_final_x4,m_final_y4);
		string new_poly_pts = new_poly.get_spec();
		m_lane_polys.push_back(new_poly_pts);

		string tss_type = seglist.get_label();
		if((tss_type == "inbound lane")||(tss_type == "inbound")){
      		new_poly.set_color("vertex", "red");
     		new_poly.set_color("edge", "red");
      		new_poly.set_color("fill", "red");
      		new_poly.set_transparency(0.05);
      		new_poly.set_edge_size(1);
      		new_poly.set_vertex_size(2);
      		string inbound_specs = new_poly.get_spec();
      		m_inbound_lanes.push_back(inbound_specs);	
		}

		if((tss_type == "outbound lane")||(tss_type == "outbound")){
      	    new_poly.set_color("vertex", "green");
     	    new_poly.set_color("edge", "green");
     	    new_poly.set_color("fill", "green");
     	    new_poly.set_transparency(0.025);
     	    new_poly.set_edge_size(1);
    	    new_poly.set_vertex_size(2);
    	    string outbound_specs = new_poly.get_spec();
    	    m_outbound_lanes.push_back(outbound_specs);
		}

		string new_specs = new_poly.get_spec();
		m_tss_polygons.push_back(new_specs);

	}
}

//-----------------------------------------------------------
// Procedure: ForceConcatenate

void TrafficScheme::ForceConcatenate(vector<string> polys, vector<string> seglists)
{
	if((polys.size() == 0)||(seglists.size() == 0))
		return;
	for(int s=0; s<seglists.size(); s++){
		XYSegList current_seglist = string2SegList(seglists[s]);
		int lane_objs = current_seglist.size()-1;
		for(int i=0; i<lane_objs; i++){
			m_init_dist1 = 1000;
			m_init_dist2 = 1000;
			m_x1 = current_seglist.get_vx(i);
			m_y1 = current_seglist.get_vy(i);

			m_x2 = current_seglist.get_vx(i+1);
			m_y2 = current_seglist.get_vy(i+1);
			string heading = doubleToString(relAng(m_x1,m_y1,m_x2,m_y2));
			m_poly_headings.push_back(heading);

			for(int j=0; j<polys.size(); j++){
			XYPolygon poly = string2Poly(polys[j]);	
			XYSegList curr_poly1 = poly.exportSegList(m_x1,m_y1);
			m_curr_x4 = curr_poly1.get_vx(0);
			m_curr_y4 = curr_poly1.get_vy(0);
			m_curr_dist1 = distPointToPoint(m_x1,m_y1,m_curr_x4,m_curr_y4);

			XYSegList curr_poly2 = poly.exportSegList(m_x2,m_y2);
			m_curr_x3 = curr_poly2.get_vx(0);
			m_curr_y3 = curr_poly2.get_vy(0);
			m_curr_dist2 = distPointToPoint(m_x2,m_y2,m_curr_x3,m_curr_y3);
		
			if(m_curr_dist1 < m_init_dist1){
				m_final_x4 = m_curr_x4;
				m_final_y4 = m_curr_y4;			
				m_init_dist1 = m_curr_dist1;
			}

			if(m_curr_dist2 < m_init_dist2){
				m_final_x3 = m_curr_x3;
				m_final_y3 = m_curr_y3;				
				m_init_dist2 = m_curr_dist2;
			}
		}

		XYPolygon new_poly;
		new_poly.add_vertex(m_x1,m_y1);
		new_poly.add_vertex(m_x2,m_y2);		
		new_poly.add_vertex(m_final_x3,m_final_y3);
		new_poly.add_vertex(m_final_x4,m_final_y4);
		string new_poly_pts = new_poly.get_spec();
		m_lane_polys.push_back(new_poly_pts);

		string tss_type = current_seglist.get_label();
		if((tss_type == "inbound lane")||(tss_type == "inbound")){
      		new_poly.set_color("vertex", "red");
     		new_poly.set_color("edge", "red");
      		new_poly.set_color("fill", "red");
      		new_poly.set_transparency(0.05);
      		new_poly.set_edge_size(1);
      		new_poly.set_vertex_size(2);
      		string inbound_specs = new_poly.get_spec();
      		m_inbound_lanes.push_back(inbound_specs);	
		}

		if((tss_type == "outbound lane")||(tss_type == "outbound")){
      	    new_poly.set_color("vertex", "green");
     	    new_poly.set_color("edge", "green");
     	    new_poly.set_color("fill", "green");
     	    new_poly.set_transparency(0.025);
     	    new_poly.set_edge_size(1);
    	    new_poly.set_vertex_size(2);
    	    string outbound_specs = new_poly.get_spec();
    	    m_outbound_lanes.push_back(outbound_specs);
		}
		string new_specs = new_poly.get_spec();
		m_tss_polygons.push_back(new_specs);
	}
	}
}