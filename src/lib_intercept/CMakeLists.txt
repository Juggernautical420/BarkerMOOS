#--------------------------------------------------------
# The CMakeLists.txt for:                    lib_intercept
# Author(s):                                Jason Barker
#--------------------------------------------------------

SET(SRC
  SegIntercept.cpp
  SegListExtrapolator.cpp
  SegListContact.cpp
  SegListContactSet.cpp
 
 
)

SET(HEADERS
  SegIntercept.h
  SegListEdit.h
  SegListExtrapolator.h
  SegListContact.h
  SegListContactSet.h
)

# Build Library
ADD_LIBRARY(intercept ${SRC})

