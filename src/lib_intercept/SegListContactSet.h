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


class SegListContactSet
{
 public:
  SegListContactSet();
  virtual ~SegListContactSet() {}


// Setters
void addSegListContact(SegListContact seglistcontact);


//Getters
unsigned int size() const  {return(m_seglist_contacts.size());}
std::vector<std::string> extrapolate_all(double time);
SegListContact get_contact(unsigned int) const;



protected:

std::vector<SegListContact> m_seglist_contacts;




};

#endif
