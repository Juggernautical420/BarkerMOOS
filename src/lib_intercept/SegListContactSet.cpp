/*****************************************************************/
/*    NAME: Jason Barker                                      */
/*    ORGN: MIT   												 */
/*    FILE: SegListContactSet.cpp                                         */
/*    DATE: Apr 17, 2020                                       */
/*****************************************************************/

#include "MBUtils.h"
#include <algorithm>
#include <iostream>
#include "SegListContactSet.h"
#include "GeomUtils.h"

using namespace std;


//-----------------------------------------------------------
// Constructor

SegListContactSet::SegListContactSet()
{

}

//-----------------------------------------------------------
// Procedure: addSegListContact

void SegListContactSet::addSegListContact(SegListContact seglistcontact)
{
	m_seglist_contacts.push_back(seglistcontact);
}

//-----------------------------------------------------------
// Procedure: extrapolate_all

vector<string> SegListContactSet::extrapolate_all(double time)
{
	vector<string> pointvector;
	for(int i=0; i<m_seglist_contacts.size(); i++){
		SegListContact current = m_seglist_contacts[i];
		XYPoint predict = current.extrapolate_point(time);
		string prediction = predict.get_spec();
		pointvector.push_back(prediction);
	}
	return(pointvector);
}

//---------------------------------------------------------------
// Procedure: get_contact

SegListContact SegListContactSet::get_contact(unsigned int i) const
{
  if(i<m_seglist_contacts.size())
     return(m_seglist_contacts[i]);
   else{
   	SegListContact null_contact;
   	return(null_contact);
   }
    
}

//---------------------------------------------------------------
// Procedure: minDistContact

string SegListContactSet::minDistContact(double x, double y, double time)
{
	string pointspec;
	m_init_dist = 1000; 
	vector<string> predict_points = extrapolate_all(time);
	for(int i=0; i<predict_points.size(); i++){
		XYPoint ownship;
		ownship.set_vertex(x,y);
		XYPoint contact = string2Point(predict_points[i]);
		m_curr_dist = distPointToPoint(ownship, contact);

		if(m_curr_dist < m_init_dist){
			pointspec = contact.get_spec();
			m_init_dist = m_curr_dist;
			m_final_dist = m_curr_dist;
		}
	}

	return(pointspec);
}