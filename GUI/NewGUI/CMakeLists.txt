Project (TTTNewGUI)

FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)

set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
add_definitions(${Qt5Widgets_DEFINITIONS})

# some .ui files sqlnewprojectdialog.ui sqlopenprojectdialog.ui
SET(TTTNewGUI_UIS tttmainwindow.ui 
    )

# another list, this time it includes all header files that should be treated with moc
SET( TTTNewGUI_MOC_HDRS
  tttmainwindow.h
  )

SET(TTTGUI_SRCS
  tttmainwindow.cpp
   )
   

# this will run uic on .ui files:
include_directories( ../../Modules/IO/include/)
include_directories( ../../Modules/Core/include/)
include_directories( ../../Modules/Drawers/include/)
include_directories( ../../Modules/Commands/DataCasting/include/)
include_directories( ../../Modules/Commands/Plateness/include/)
include_directories( ../../Modules/Commands/Vertexness/include/)
include_directories( ../../Modules/Commands/ObjectnessMeasurementBase/include/)
include_directories( ../../Modules/Commands/AnisotropicDiffusion/include/)
include_directories( ../../Modules/Commands/VertexLocation/include/)
include_directories( ../../Modules/Commands/AdherensJunctionSegmentationDijkstra/include/ )
include_directories( ../../Modules/Commands/CellGraph/include/ )
include_directories( ../../Modules/Commands/Base/include/)

include_directories( ../../Modules/Commands/Tracking/include/)
include_directories( ../../Modules/Commands/Ellipses/include/)
include_directories( ../../Modules/Commands/ComputeDomains/include/ )
include_directories( ../../Modules/Commands/Tectonics/include/ )
include_directories( ../../Modules/Features/include/)

qt5_wrap_ui(UI_HDRS tttmainwindow.ui tttsqlopenprojectdialog.ui tttsqlnewprojectdialog.ui tttexplorer.ui tttpreferences.ui)

add_executable(TTTNewGui main.cpp tttmainwindow.cpp tttsqlnewprojectdialog.cpp tttsqlopenprojectdialog.cpp tttexplorer.cpp tttpreferences.cpp ${UI_HDRS} VertexAdditionInteractor.cpp VertexSelectionInteractor.cpp VertexAdditionToPrimalInteractor.cpp EdgeSelectionInteractor.cpp EdgeAdditionInteractor.cpp) 
qt5_use_modules(TTTNewGui Widgets)
qt5_use_modules(TTTNewGui Sql)

target_link_libraries(TTTNewGui 
					${VTK_LIBRARIES} 
					${ITK_LIBRARIES} 
					mysqlcppconn
					TTTDrawers 
					TTTPlateness 
					TTTVertexness 
					TTTDataCasting 
					TTTAnisotropicDiffusion 
					TTTVertexLocation
					TTTAdherensJunctionSegmentationDijkstra
					TTTCellGraph
					TTTTracking
					TTTTectonics
					TTTComputeDomains
					
					Cbc CbcSolver Cgl Clp CoinUtils bz2   OsiClp Osi ${ZLIB_LIBRARIES} lapack
					)

