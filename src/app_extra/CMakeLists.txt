#--------------------------------------------------------
# The CMakeLists.txt for:                     app_extra
# Author(s):                                Jason Barker
#--------------------------------------------------------


SET(SYSTEM_LIBS m)

SET(SRC 
  main.cpp
  )

ADD_EXECUTABLE(extra ${SRC})

TARGET_LINK_LIBRARIES(extra
  mbutil
  traffic
  intercept
  geometry
  ${SYSTEM_LIBS}
)
  


