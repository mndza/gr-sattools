INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_SATTOOLS sattools)

FIND_PATH(
    SATTOOLS_INCLUDE_DIRS
    NAMES sattools/api.h
    HINTS $ENV{SATTOOLS_DIR}/include
        ${PC_SATTOOLS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    SATTOOLS_LIBRARIES
    NAMES gnuradio-sattools
    HINTS $ENV{SATTOOLS_DIR}/lib
        ${PC_SATTOOLS_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/sattoolsTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SATTOOLS DEFAULT_MSG SATTOOLS_LIBRARIES SATTOOLS_INCLUDE_DIRS)
MARK_AS_ADVANCED(SATTOOLS_LIBRARIES SATTOOLS_INCLUDE_DIRS)
