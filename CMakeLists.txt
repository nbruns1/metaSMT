PROJECT(metaSMT)
cmake_minimum_required(VERSION 2.8.9)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
	add_definitions(-std=c++11)
else()
	message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} does not support -std=c++11. Please use a different C++ compiler.")
endif()

include(CheckIncludeFileCXX)
include(FeatureSummary)

include(CTest)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules )

set(Boost_NO_BOOST_CMAKE true)
set(Boost_NO_SYSTEM_PATHS true)
set(Boost_ADDITIONAL_VERSIONS "1.50.0")
find_package(Boost)

if(NOT ${Boost_FOUND})
  message(FATAL_ERROR "Boost could not be found.")
endif()

set_package_info(Boost "Portable C++ libraries")
include_directories(${Boost_INCLUDE_DIRS})

list(INSERT CMAKE_PREFIX_PATH 0 "${CMAKE_SOURCE_DIR}/cmake")

# Set the default version number, but try to calculate
# a proper version from git.
SET(metaSMT_VERSION 4)
include(cmake/get_git_version.cmake)

include(cmake/build_type.cmake)
include(cmake/default_compliler_flags.cmake)
include(cmake/config_file.cmake)

###  
# Options to force disable certain solvers. 
# set all solvers to "on" with good support and experimental or
# work in progress to "off"
###
option(metaSMT_DOC_Doxygen    "try to build doxygen documentation" off)
option(metaSMT_ENABLE_TESTS   "build tests (recommended)" on)

set(metaSMT_CONFIG_DIR "share/metaSMT" CACHE PATH 
  "where to install CMake and Make Config files")

if ( NOT TIMEOUT )
  set(TIMEOUT 5 CACHE STRING "The timeout for tests")
endif()
find_package(Threads REQUIRED)
find_package(GMP REQUIRED)
find_package(Z3 REQUIRED)
set(Z3_LIBRARIES ${Z3_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

config_find(Z3 ${Z3_DIR})
list(APPEND metaSMT_LIBS ${Z3_LIBRARIES} ${GMP_LIBRARIES})
list(APPEND metaSMT_INCLUDES ${Z3_INCLUDE_DIR} ${GMP_INCLUDE_DIR})

if (metaSMT_DOC_Doxygen)
  find_package(Doxygen)
else ()
  set(Doxygen_FOUND 0)
endif ()

############################################################
############# RPATH ########################################
############################################################

include(cmake/library_rpath.cmake)
add_dependent_libraries(${metaSMT_LIBS})


############################################################
############# subdirectories ###############################
############################################################

add_subdirectory(src)
add_subdirectory(doc)

if(metaSMT_ENABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

############################################################
########## generate cmake config files #####################
############################################################

generate_config_files()

Feature_Summary(WHAT ALL)

############################################################
##                CPack settings                          ##
############################################################


# CPack version numbers for release tarball name.
set(CPACK_PACKAGE_VERSION_MAJOR ${metaSMT_VERSION})
#set(CPACK_PACKAGE_VERSION_MINOR 0)
#set(CPACK_PACKAGE_VERSION_PATCH 0)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SMT programming library")
set(CPACK_PACKAGE_VENDOR "metaSMT development team")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(CPACK_GENERATOR TGZ)
set(
CPACK_SOURCE_PACKAGE_FILE_NAME
"metaSMT-${metaSMT_VERSION}"
CACHE INTERNAL "tarball basename"
)
set(CPACK_SOURCE_GENERATOR TGZ TBZ2)

set(CPACK_SOURCE_IGNORE_FILES
"/\\\\.git"
"/deps"
"/build.*/"
"/root"
)
include(CPack)


############################################################
############################################################

# vim: ft=cmake:ts=2:sw=2:expandtab
