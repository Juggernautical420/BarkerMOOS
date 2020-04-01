#--------------------------------------------------------
# The CMakeLists.txt for:                     app_foobar
# Author(s):                                Mike Benjamin
#--------------------------------------------------------


SET(SYSTEM_LIBS m)

SET(SRC 
  main.cpp
  )

ADD_EXECUTABLE(foobar ${SRC})

TARGET_LINK_LIBRARIES(foobar 
  mbutil
  traffic
  ${SYSTEM_LIBS}
)
  


