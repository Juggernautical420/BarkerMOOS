/*****************************************************************/
/*    NAME: Jason Barker                                      */
/*    ORGN: MIT   												 */
/*    FILE: SegListContactSet.h                                          */
/*    DATE: Apr 17, 2020                                       */
/*****************************************************************/

#ifndef SEGLIST_CONTACT_SET_HEADER
#define SEGLIST_CONTACT_SET_HEADER

#include <string>
#include <vector>
#include "SegListContact.h"
#include "XYPoint.h"
#include "XYSegList.h"
#include "XYObject.h"
#include "XYFormatUtilsPoint.h"


class SegListContactSet
{
 public:
  SegListContactSet();
  virtual ~SegListContactSet() {}


// Setters
void addSegListContact(SegListContact seglistcontact);


//Getters
unsigned int size() const  {return(m_seglist_contacts.size());}
SegListContact get_contact(unsigned int) const;

/// Calculations // meh!!!! not dynamic
std::vector<std::string> extrapolate_all(double time);
std::string minDistContact(double x, double y, double time); //with a vector of contacts, this will return the 
															 // string spec of the closest contact
double minDist()  {return(m_final_dist);};


protected:

std::vector<SegListContact> m_seglist_contacts;
double m_init_dist;
double m_min_dist;
double m_curr_dist;
double m_final_dist;




};

#endif
