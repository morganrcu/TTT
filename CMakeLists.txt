PROJECT(TTT)

cmake_minimum_required(VERSION 2.8)

SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS} "-Wall -std=c++0x -fpermissive -fPIC " )

INCLUDE(CPack)
INCLUDE(ExternalProject)


FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS graph)

FIND_PACKAGE(Qt5Widgets REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})


FIND_PACKAGE(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
add_definitions(${Qt5Core_DEFINITIONS})

FIND_PACKAGE(Qt5Gui REQUIRED)
add_definitions(${Qt5Gui_DEFINITIONS})

FIND_PACKAGE(Qt5Sql REQUIRED)
include_directories(${Qt5Sql_INCLUDE_DIRS})
add_definitions(${Qt5Sql_DEFINITIONS})





INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})



OPTION(USE_SYSTEM_ZLIB "Use system Zlib" ON)
OPTION(USE_SYSTEM_BZIP2 "Use system Bzip2" ON)
OPTION(USE_SYSTEM_LAPACK "Use system Lapack" ON)
OPTION(USE_SYSTEM_COINCBC "Use system Coin-cbc" ON)


IF(USE_SYSTEM_COINCBC)
   
ELSE(USE_SYSTEM_COINCBC)
	
	ExternalProject_add(coin-cbc PREFIX ${CMAKE_BINARY_DIR}/coin-cbc/
                            URL http://www.coin-or.org/download/source/Cbc/Cbc-2.8.10.tgz
                            CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/coin-cbc/src/coin-cbc/configure --enable-static --prefix=${CMAKE_BINARY_DIR}/coin-cbc                           
                            INSTALL_DIR ${CMAKE_BINARY_DIR}/coin-cbc-inst
    )
                            
                            
	SET(COIN_CBC_LIBRARIES  ${CMAKE_BINARY_DIR}/coin-cbc/lib/libCbc.a
                        ${CMAKE_BINARY_DIR}/coin-cbc/lib/libCbcSolver.a
                        ${CMAKE_BINARY_DIR}/coin-cbc/lib/libCgl.a
                        ${CMAKE_BINARY_DIR}/coin-cbc/lib/libClp.a
                        ${CMAKE_BINARY_DIR}/coin-cbc/lib/libCoinUtils.a
                        ${CMAKE_BINARY_DIR}/coin-cbc/lib/libOsiCbc.a
                        ${CMAKE_BINARY_DIR}/coin-cbc/lib/libOsiClp.a
                        ${CMAKE_BINARY_DIR}/coin-cbc/lib/libOsi.a)                       

	include_directories(${CMAKE_BINARY_DIR}/coin-cbc/include)
ENDIF(USE_SYSTEM_COINCBC)

MESSAGE(${COIN_CBC_LIBRARIES})
IF(USE_SYSTEM_ZLIB)
    find_package( ZLIB REQUIRED )
    
else(USE_SYSTEM_ZLIB)
	
	ExternalProject_add(ZLIB PREFIX ${CMAKE_BINARY_DIR}/zlib                             
						URL http://zlib.net/zlib-1.2.8.tar.gz
						CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/zlib-inst -DINSTALL_BIN_DIR=${CMAKE_BINARY_DIR}/zlib-inst/bin -DINSTALL_INC_DIR=${CMAKE_BINARY_DIR}/zlib-inst/include  -DINSTALL_LIB_DIR=${CMAKE_BINARY_DIR}/zlib-inst/lib -DINSTALL_MAN_DIR=${CMAKE_BINARY_DIR}/zlib-inst/man -DINSTALL_PKGCONFIG_DIR=${CMAKE_BINARY_DIR}/zlib-inst/share/pkgconfig	                             
						INSTALL_DIR PREFIX=${CMAKE_BINARY_DIR}/zlib-inst
						) 
	SET(ZLIB_LIBRARIES ${CMAKE_BINARY_DIR}/zlib-inst/lib/libz.a)						
endif(USE_SYSTEM_ZLIB)

IF(USE_SYSTEM_BZIP2)
      find_package( BZip2 REQUIRED )
else(USE_SYSTEM_BZIP2)
	
	EXTERNALPROJECT_ADD(bzip2
			PREFIX ${CMAKE_BINARY_DIR}/bzip2
			URL http://www.bzip.org/1.0.6/bzip2-1.0.6.tar.gz    
			CONFIGURE_COMMAND ""    
			BUILD_COMMAND make PREFIX=${CMAKE_BINARY_DIR} SHARED= XCFLAGS=-I${CMAKE_BINARY_DIR}/include XLDFLAGS=-L${CMAKE_BINARY_DIR}/lib XLIBS=-ldl    
			INSTALL_COMMAND make install PREFIX=${CMAKE_BINARY_DIR}/bzip2-inst SHARED=    
			BUILD_IN_SOURCE 1) 
			
			                        
	SET(BZIP2_LIBRARIES  ${CMAKE_BINARY_DIR}/bzip2-inst/lib/libbz2.a)
                        
endif(USE_SYSTEM_BZIP2)



IF(USE_SYSTEM_LAPACK)
    find_package( LAPACK REQUIRED ) 
else(USE_SYSTEM_LAPACK)
	ExternalProject_add(lapack PREFIX ${CMAKE_BINARY_DIR}/lapack/
					      URL http://www.netlib.org/clapack/clapack-3.2.1-CMAKE.tgz
					      CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/lapack-inst
					      INSTALL_COMMAND ""
					      INSTALL_DIR ${CMAKE_BINARY_DIR}/lapack-inst
					      )
	
	SET(LAPACK_LIBRARIES 
						 ${CMAKE_BINARY_DIR}/lapack/src/lapack-build/SRC/liblapack.a 
					     ${CMAKE_BINARY_DIR}/lapack/src/lapack-build/BLAS/SRC/libblas.a  
	                     
	                     ${CMAKE_BINARY_DIR}/lapack/src/lapack-build/F2CLIBS/libf2c/libf2c.a
						 )

endif(USE_SYSTEM_LAPACK)


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

#add_subdirectory(Modules/Assesment/VertexLocationAssesment)
#add_subdirectory(Modules/Assesment/SegmentationAssesment)
add_subdirectory(Modules/Assesment/TrackingAssesment)
add_subdirectory(Modules/Assesment/TrackingOptimizing)
add_subdirectory(Modules/TrackAndDraw)
#add_subdirectory(Modules/PaperFigures/VertexLocationDrawing)
#add_subdirectory(Modules/PaperFigures/SegmentationDrawing)
add_subdirectory(Modules/PaperFigures/PrimalDrawing)
add_subdirectory(Modules/PaperFigures/TrackedCellsDrawing)
#add_subdirectory(Modules/PaperFigures/SelectedCellsVolumeDrawing)
add_subdirectory(TTTGUI)
add_subdirectory(Modules/ScaleSpaceExplorer)
#add_subdirectory(Modules/TrackExplorer)
#add_subdirectory(Modules/ObservationExplorer)
add_subdirectory(Modules/TrackVolume)
add_subdirectory(Modules/TrackAnnotation)
#add_subdirectory(VertexRelabel)
add_subdirectory(Modules/Registration)
add_subdirectory(Modules/RemoveOverlap)
add_subdirectory(Modules/Deconvolution)
