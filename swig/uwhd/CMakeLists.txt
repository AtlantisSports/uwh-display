FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs 2.7 REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_SWIG_FLAGS "-c++")
set(SWIG_CXX_EXTENSION "cpp")
set(CMAKE_SWIG_OUTDIR ${CMAKE_BINARY_DIR}/lib)

SET(${SWIG_MODULE_console_EXTRA_DEPS}
    ${CMAKE_SOURCE_DIR}/include/*)
SET_SOURCE_FILES_PROPERTIES(console.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(console.i PROPERTIES SWIG_FLAGS "")
SWIG_ADD_MODULE(console python console.i)
SWIG_LINK_LIBRARIES(console ${PYTHON_LIBRARIES} uwhdconsole uwhdmodel)

if(UWHD_HAVE_LIBXBEE3)
  set(UWHD_COMMS_LIB ${LIBXBEE3})
endif()

FIND_LIBRARY(LIBRT rt)
if(LIBRT)
  set(UWHD_SUPPORT_LIBS ${LIBRT} m pthread)
else()
  set(UWHD_SUPPORT_LIBS m pthread)
endif()

if(UWHD_BUILD_DISPLAY)
  SET(SWIG_MODULE_uwhd_EXTRA_DEPS
      ${CMAKE_CURRENT_SOURCE_DIR}/canvas.i
      ${CMAKE_CURRENT_SOURCE_DIR}/console.i
      ${CMAKE_CURRENT_SOURCE_DIR}/display.i
      ${CMAKE_CURRENT_SOURCE_DIR}/model.i
      ${CMAKE_CURRENT_SOURCE_DIR}/sync.i
      ${CMAKE_SOURCE_DIR}/include/*)
  SET_SOURCE_FILES_PROPERTIES(uwhd.i PROPERTIES CPLUSPLUS ON)
  SET_SOURCE_FILES_PROPERTIES(uwhd.i PROPERTIES SWIG_FLAGS "")
  SWIG_ADD_MODULE(uwhd python uwhd.i)
  SWIG_LINK_LIBRARIES(uwhd
                      ${PYTHON_LIBRARIES}
                      uwhdsync
                      uwhdmodel
                      uwhdconsole
                      uwhddisplay
                      uwhdcanvas
                      uwhdmodel
                      ${LIBRGBMATRIX}
                      ${UWHD_COMMS_LIB}
                      ${UWHD_SUPPORT_LIBS})
endif()

SET(SWIG_MODULE_uwhdnodisp_EXTRA_DEPS
    ${CMAKE_CURRENT_SOURCE_DIR}/canvas.i
    ${CMAKE_CURRENT_SOURCE_DIR}/console.i
    ${CMAKE_CURRENT_SOURCE_DIR}/model.i
    ${CMAKE_CURRENT_SOURCE_DIR}/sync.i
    ${CMAKE_SOURCE_DIR}/include/*)
SET_SOURCE_FILES_PROPERTIES(uwhdnodisp.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(uwhdnodisp.i PROPERTIES SWIG_FLAGS "")
SWIG_ADD_MODULE(uwhdnodisp python uwhdnodisp.i)
SWIG_LINK_LIBRARIES(uwhdnodisp
                    ${PYTHON_LIBRARIES}
                    uwhdsync
                    uwhdmodel
                    uwhdconsole
                    uwhdcanvas
                    uwhdmodel
                    ${UWHD_COMMS_LIB}
                    ${UWHD_SUPPORT_LIBS})

