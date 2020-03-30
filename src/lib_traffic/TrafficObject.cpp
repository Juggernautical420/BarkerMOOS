/*****************************************************************/
/*    NAME: Jason Barker                                      */
/*    ORGN: MIT   												 */
/*    FILE: TrafficObject.cpp                                           */
/*    DATE: Mar 30, 2020                                       */
/*****************************************************************/

#include <cstdlib> 
#include <iostream> 
#include "TrafficObject.h"
#include "MBUtils.h"

using namespace std;


//----------------------------------------------------------
// Constructor

TrafficObject::TrafficObject()
{

}

//----------------------------------------------------------
// Procedure: print()

void TrafficObject::print() const
{
  cout << "Traffic Type= " << m_type   << endl;

  if(m_poly_type != "")
    cout << "Polygon Type  = " << m_poly_type  << endl;

  if(m_points != "") 
    cout << "Points  = " << m_points  << endl;

  if(m_pname != "") 
    cout << "Label  = " << m_pname  << endl;

  if(m_start != "") 
    cout << "Starting Point  = " << m_start  << endl;
}