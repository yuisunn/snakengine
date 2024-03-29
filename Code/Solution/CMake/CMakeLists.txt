PROJECT(SnakEngine)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules/")
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.7)

# Main settings and dependencies
SET(KLAYGE_CORELIB_NAME KlayGE_Core)  # Dependency of all other targets
FIND_PATH(KLAYGE_PROJECT_DIR klayge_logo.ico PATHS ../../)

SET(3RDPARTY_DIR "$ENV{KLAYGE_HOME}" CACHE PATH "Location of 3rdparty dependencies")
FIND_PACKAGE(Doxygen)
FIND_PACKAGE(OpenGL)
FIND_PACKAGE(Boost 1.40.0 COMPONENTS filesystem system thread signals)
FIND_PACKAGE(PythonLibs)
FIND_PACKAGE(DirectX)
FIND_PACKAGE(DirectShow)
FIND_PACKAGE(Cg)
FIND_PACKAGE(OpenAL)
FIND_PACKAGE(OggVorbis)
FIND_PACKAGE(glloader)

FIND_PATH(RAPIDXML_INCLUDE_DIR rapidxml/rapidxml.hpp
          PATHS ${3RDPARTY_DIR}/include /usr/include /usr/local/include)
GET_FILENAME_COMPONENT( GLLOADER_LIBRARY_DIR "${GLLOADER_LIBRARY}" PATH )
GET_FILENAME_COMPONENT( CG_LIBRARY_DIR "${CG_LIBRARY}" PATH )

INCLUDE_DIRECTORIES(${CMAKE_INCLUDE_PATH} ${RAPIDXML_INCLUDE_DIR})
LINK_DIRECTORIES(${CMAKE_LIBRARY_PATH})

