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

using namespace std;


//-----------------------------------------------------------
// Constructor

SegListContactSet::SegListContactSet()
{

}

//-----------------------------------------------------------
// Procedure: addSegListContact

bool SegListContactSet::addSegListContact(SegListContact seglistcontact)
{
	m_seglist_contacts.push_back(seglistcontact);
	return(true);
}

//-----------------------------------------------------------
// Procedure: extrapolate_all

vector<XYPoint> SegListContactSet::extrapolate_all(double time)
{
	vector<XYPoint> pointvector;
	for(int i=0; i<m_seglist_contacts.size(); i++){
		SegListContact current = m_seglist_contacts[i];
		XYPoint prediction = current.extrapolate_point(time);
		pointvector.push_back(prediction);
	}
	return(pointvector);
}