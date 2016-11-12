if(UWHD_BUILD_CLIENT AND
   UWHD_BUILD_DISPLAY)
  add_subdirectory(uwhdc)
endif()

if(UWHD_BUILD_DISPLAY)
  add_subdirectory(uwhdi)
endif()

if(UWHD_BUILD_SERVER AND
   UWHD_BUILD_DISPLAY)
  add_subdirectory(uwhdd)
endif()

add_subdirectory(display-check)
