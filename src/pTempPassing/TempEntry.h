/************************************************************/
/*    NAME: Team Awesome                                              */
/*    ORGN: MIT                                             */
/*    FILE: TempEntry.h                                          */
/*    DATE: May 9, 2019                             */
/************************************************************/

// Message Format: UCTD_MSMNT_REPORT="vname=archie,utc=46716928330.8,x=21.1,y=-135.3,temp=26.69"


#ifndef TEMP_ENTRY_HEADER
#define TEMP_ENTRY_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <vector>
#include <iterator>
#include <cstdlib>
#include <cstdint>
#include <stdint.h>
#include "MBUtils.h"
#include "NodeMessage.h"
#include "BuildUtils.h"
#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include <iterator>
#include "ACTable.h"

using namespace std;

class TempEntry
{
public:

	TempEntry();
	~TempEntry();

	void parseMessage(string v);
	void setVname(string v);
	void setUTC(double v)						{m_utc = v;};
	void setX(double v)							{m_x = v;};
	void setY(double v)							{m_y = v;};
	void setTemp(double v)						{m_temp = v;};
	void setReceivedIndex(unsigned int v)	 	{m_received_index = v;};
	void setSentFlag(bool v)					{m_sent_flag = v;};
	bool sent() 								{return(m_sent_flag);};
	string genMessage();
	

protected:
	string m_vname;
	string m_other_vname;
	double m_utc;
	double m_x;
	double m_y;
	double m_temp;
	double m_received_index;
	bool m_sent_flag;

};

#endif