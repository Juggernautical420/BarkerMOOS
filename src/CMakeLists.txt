##############################################################################
# FILE:        moos-ivp-extend/src/CMakeLists.txt
# DATE:        2010/09/07
# DESCRIPTION: CMakeLists.txt file for the moos-ivp-extend source directory
##############################################################################


#============================================================================
# Add the libraries in the current directory to the include path
#============================================================================
FILE(GLOB LOCAL_LIBRARY_DIRS ./lib_*)
INCLUDE_DIRECTORIES(${LOCAL_LIBRARY_DIRS})

#============================================================================
# List the subdirectories to build...
#============================================================================
ADD_SUBDIRECTORY(lib_behaviors-test)
ADD_SUBDIRECTORY(lib_intercept)
#ADD_SUBDIRECTORY(pXRelayTest)
#ADD_SUBDIRECTORY(pExampleApp)
#ADD_SUBDIRECTORY(pPrimeFactor)
#ADD_SUBDIRECTORY(uFldHazardMgrX)
#ADD_SUBDIRECTORY(pFoobar)
#ADD_SUBDIRECTORY(pOdometry)
#ADD_SUBDIRECTORY(pCommunicationAngle_jbbarker)
#ADD_SUBDIRECTORY(pPointAssign)
#ADD_SUBDIRECTORY(pGenPath)
#ADD_SUBDIRECTORY(pTempGenPath)
#ADD_SUBDIRECTORY(pTempPassing)
ADD_SUBDIRECTORY(pSegPassing)
ADD_SUBDIRECTORY(pSegListIntercept)
ADD_SUBDIRECTORY(pSegDetailing)
ADD_SUBDIRECTORY(pTransitAlpha)
ADD_SUBDIRECTORY(pTSSCompliance)


##############################################################################
#                           END of CMakeLists.txt
##############################################################################
