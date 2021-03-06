#--------------------------------------------------------
# The CMakeLists.txt for:              lib_behaviors-test
# Author(s):                                
#--------------------------------------------------------

# Set System Specific Libraries
if (${WIN32})
  # Windows Libraries
  SET(SYSTEM_LIBS
      )
else (${WIN32})
  # Linux and Apple Libraries
  SET(SYSTEM_LIBS
      m )
endif (${WIN32})


MACRO(ADD_BHV BHV_NAME)
  ADD_LIBRARY(${BHV_NAME} SHARED ${BHV_NAME}.cpp)
  TARGET_LINK_LIBRARIES(${BHV_NAME} 
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} )
ENDMACRO(ADD_BHV)


#--------------------------------------------------------
#                                      BHV_SimpleWaypoint
#--------------------------------------------------------
ADD_LIBRARY(BHV_SimpleWaypoint SHARED 
   BHV_SimpleWaypoint.cpp AOF_SimpleWaypoint.cpp)
TARGET_LINK_LIBRARIES(BHV_SimpleWaypoint
   helmivp
   behaviors 
   ivpbuild 
   logic 
   ivpcore 
   bhvutil 
   mbutil 
   geometry 
   ${SYSTEM_LIBS} )


 #--------------------------------------------------------
 #                                      		 BHV_Pulse
 #--------------------------------------------------------
 ADD_LIBRARY(BHV_Pulse SHARED 
    BHV_Pulse.cpp)
 TARGET_LINK_LIBRARIES(BHV_Pulse
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} )

#--------------------------------------------------------
#                                           BHV_ZigLeg 
#--------------------------------------------------------
 ADD_LIBRARY(BHV_ZigLeg SHARED 
    BHV_ZigLeg.cpp)
 TARGET_LINK_LIBRARIES(BHV_ZigLeg
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} )
#--------------------------------------------------------
#                                     BHV_InitialSweep 
#--------------------------------------------------------
 ADD_LIBRARY(BHV_InitialSweep SHARED 
    BHV_InitialSweep.cpp AOF_SimpleWaypoint.cpp)
 TARGET_LINK_LIBRARIES(BHV_InitialSweep
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} ) 
#--------------------------------------------------------
#                                           BHV_Tripwire 
#--------------------------------------------------------
 ADD_LIBRARY(BHV_Tripwire SHARED 
    BHV_Tripwire.cpp)
 TARGET_LINK_LIBRARIES(BHV_Tripwire
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} )  
#--------------------------------------------------------
#                                         BHV_FrontSurvey 
#--------------------------------------------------------
 ADD_LIBRARY(BHV_FrontSurvey SHARED 
    BHV_FrontSurvey.cpp)
 TARGET_LINK_LIBRARIES(BHV_FrontSurvey
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} ) 
#--------------------------------------------------------
#                                         BHV_DetailSurvey 
#--------------------------------------------------------
 ADD_LIBRARY(BHV_DetailSurvey SHARED 
    BHV_DetailSurvey.cpp)
 TARGET_LINK_LIBRARIES(BHV_DetailSurvey
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} ) 

#--------------------------------------------------------
#                                         BHV_WaypointX
#--------------------------------------------------------
 ADD_LIBRARY(BHV_WaypointX SHARED 
    BHV_WaypointX.cpp)
 TARGET_LINK_LIBRARIES(BHV_WaypointX
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} )      
