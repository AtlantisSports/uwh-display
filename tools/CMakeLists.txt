if(UWHD_BUILD_CLIENT AND
   UWHD_BUILD_DISPLAY)
  add_subdirectory(uwhdc)
endif()

add_subdirectory(test-gamemodel)

if(UWHD_BUILD_DISPLAY)
  add_subdirectory(uwhdi)
endif()

if(UWHD_BUILD_SERVER AND
   UWHD_BUILD_DISPLAY)
  add_subdirectory(uwhdd)
endif()
