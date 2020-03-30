/*****************************************************************/
/*    NAME: Jason Barker                                      */
/*    ORGN: MIT   												 */
/*    FILE: TrafficObject.h                                           */
/*    DATE: Mar 30, 2020                                       */
/*****************************************************************/

#ifndef TRAFFIC_OBJECT_HEADER
#define TRAFFIC_OBJECT_HEADER

#include <string>
#include <vector>

class TrafficObject {
public:
  TrafficObject();
  virtual ~TrafficObject() {};

// Setters ///

void setTrafficType(std::string str)		{m_type=str;};
void setPolyType(std::string str)		{m_poly_type=str;};
void setPolyPts(std::string str)		{m_points=str;}; 
void setPolyName(std::string str)		{m_pname=str;};
void setStartPt(std::string str)		{m_start=str;};


// Getters //

std::string getTrafficType()	const {return(m_type);};
std::string	getPolyType()	const {return(m_poly_type);};
std::string getPolyPts()	const {return(m_points);};
std::string getPolyName()	const {return(m_pname);};
std::string getPolyStartPts()	const {return(m_start);};



void print() const;

protected:
	
std::string m_type;
std::string m_poly_type;
std::string m_points;
std::string m_pname;
std::string m_start;


};

#endif