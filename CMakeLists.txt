PROJECT(TTT)

cmake_minimum_required(VERSION 2.8)

SET(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS} -Wall)

FIND_PACKAGE(ITK REQUIRED)

INCLUDE(${ITK_USE_FILE})

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})


set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS graph)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

FIND_PACKAGE(Qt4 REQUIRED)
SET( QT_USE_QTXML TRUE )
INCLUDE(${QT_USE_FILE})


add_subdirectory(Modules/Core)
add_subdirectory(Modules/Drawers)
add_subdirectory(Modules/Commands/AdherensJunctionSegmentation)
add_subdirectory(Modules/Commands/AnisotropicDiffusion)
add_subdirectory(Modules/Commands/Base)
add_subdirectory(Modules/Commands/CellGraph)
add_subdirectory(Modules/Commands/ComputeDomains)
add_subdirectory(Modules/Commands/DataCasting)
add_subdirectory(Modules/Commands/Ellipses)
add_subdirectory(Modules/Commands/Plateness)
add_subdirectory(Modules/Commands/SurfaceSegmentation)
add_subdirectory(Modules/Commands/Tectonics)
add_subdirectory(Modules/Commands/Tracking)
add_subdirectory(Modules/Commands/VertexLocation)
add_subdirectory(Modules/Commands/Vertexness)
add_subdirectory(Modules/Interactors)
add_subdirectory(Modules/IO)
add_subdirectory(Modules/GUI)
