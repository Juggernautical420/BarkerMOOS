#--------------------------------------------------------
# The CMakeLists.txt for:                    lib_intercept
# Author(s):                                Jason Barker
#--------------------------------------------------------

SET(SRC
  SegIntercept.cpp
  SegListExtrapolator.cpp
 
 
)

SET(HEADERS
  SegIntercept.h
  SegListEdit.h
  SegListExtrapolator.h

)

# Build Library
ADD_LIBRARY(intercept ${SRC})

