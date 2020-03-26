#--------------------------------------------------------
# The CMakeLists.txt for:                    lib_intercept
# Author(s):                                Jason Barker
#--------------------------------------------------------

SET(SRC
  SegIntercept.cpp
  SegParts.cpp
  SegListExtrapolator.cpp
  XYTransitLane.cpp
  XYTrafficScheme.cpp
  XYTrafficObject.cpp
 
 
)

SET(HEADERS
  SegIntercept.h
  SegListEdit.h
  SegParts.h
  SegListExtrapolator.h
  XYTransitLane.h
  XYTrafficScheme.h
  XYTrafficObject.h

)

# Build Library
ADD_LIBRARY(intercept ${SRC})

