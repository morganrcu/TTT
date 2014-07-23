PROJECT(TTT)

cmake_minimum_required(VERSION 2.8)

SET(CMAKE_BUILD_TYPE Debug)

#SET(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS} "-Wall -O3" )
SET(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS} "-Wall" )

INCLUDE(CPack)

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

#add_subdirectory(Modules/GUI)
#add_subdirectory(ThirdParty/mysql-connector-c++-1.1.3)

add_subdirectory(Modules/Core)
add_subdirectory(Modules/Drawers)
add_subdirectory(Modules/Commands/Base)
add_subdirectory(Modules/Commands/AdherensJunctionSegmentation)
add_subdirectory(Modules/Commands/AdherensJunctionSegmentationDijkstra)
add_subdirectory(Modules/Commands/AnisotropicDiffusion)
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
add_subdirectory(Modules/Commands/LateralImageVolumeSegmentation)
add_subdirectory(Modules/Commands/CLAHE)
add_subdirectory(Modules/Commands/Surface)
add_subdirectory(Modules/Interactors)
add_subdirectory(Modules/IO)

add_subdirectory(Modules/Assesment/VertexLocationAssesment)
add_subdirectory(Modules/Assesment/SegmentationAssesment)
add_subdirectory(Modules/Assesment/TrackingAssesment)
add_subdirectory(Modules/Assesment/TrackingOptimizing)
add_subdirectory(Modules/PaperFigures/VertexLocationDrawing)
add_subdirectory(Modules/PaperFigures/SegmentationDrawing)
add_subdirectory(Modules/PaperFigures/PrimalDrawing)
add_subdirectory(GUI/NewGUI)
add_subdirectory(Modules/ScaleSpaceExplorer)
add_subdirectory(Modules/TrackExplorer)
add_subdirectory(Modules/ObservationExplorer)
add_subdirectory(Modules/TrackVolume)
add_subdirectory(Modules/TrackAnnotation)
add_subdirectory(VertexRelabel)

add_subdirectory(Modules/Registration)
add_subdirectory(Modules/RemoveOverlap)
add_subdirectory(Modules/Deconvolution)
