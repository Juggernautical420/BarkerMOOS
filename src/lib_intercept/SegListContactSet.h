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

void addSegListContact(SegListContact seglistcontact);
std::vector<XYPoint> extrapolate_all(double time);



protected:

std::vector<SegListContact> m_seglist_contacts;




};

#endif
