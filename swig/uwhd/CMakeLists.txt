FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs)
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

if(UWHD_BUILD_DISPLAY)
  SET(SWIG_MODULE_display_EXTRA_DEPS
      ${CMAKE_SOURCE_DIR}/include/*)
  SET_SOURCE_FILES_PROPERTIES(display.i PROPERTIES CPLUSPLUS ON)
  SET_SOURCE_FILES_PROPERTIES(display.i PROPERTIES SWIG_FLAGS "")
  SWIG_ADD_MODULE(display python display.i)
  SWIG_LINK_LIBRARIES(display
                      ${PYTHON_LIBRARIES}
                      uwhddisplay
                      ${LIBRGBMATRIX}
                      uwhdmodel
                      rt
                      m
                      pthread)
endif()

SET(SWIG_MODULE_model_EXTRA_DEPS
    ${CMAKE_SOURCE_DIR}/include/*)
SET_SOURCE_FILES_PROPERTIES(model.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(model.i PROPERTIES SWIG_FLAGS "")
SWIG_ADD_MODULE(model python model.i)
SWIG_LINK_LIBRARIES(model
                    ${PYTHON_LIBRARIES}
                    uwhdmodel)

SET(SWIG_MODULE_sync_EXTRA_DEPS
    ${CMAKE_SOURCE_DIR}/include/*)
SET_SOURCE_FILES_PROPERTIES(sync.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(sync.i PROPERTIES SWIG_FLAGS "")
SWIG_ADD_MODULE(sync python sync.i)
SWIG_LINK_LIBRARIES(sync
                    ${PYTHON_LIBRARIES}
                    uwhdsync
                    uwhdmodel
                    ${LIBXBEE3})

SET(SWIG_MODULE_uwhd_EXTRA_DEPS
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
                    uwhdmodel
                    ${LIBRGBMATRIX}
                    ${LIBXBEE3}
                    rt
                    m
                    pthread)

