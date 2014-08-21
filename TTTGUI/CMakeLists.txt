Project (TTTGUI)

FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
add_definitions(${Qt5Widgets_DEFINITIONS})

SET(TTTGUI_UIS    uis/tttmainwindow.ui
                  uis/tttsqlopenprojectdialog.ui 
                  uis/tttsqlnewprojectdialog.ui 
                  uis/tttexplorer.ui 
                  uis/tttpreferences.ui
)

SET(TTTGUI_MOCS   include/tttmainwindow.h
                  include/tttpreferences.h
                  include/tttsqlopenprojectdialog.h
                  include/tttsqlnewprojectdialog.h
                  include/tttexplorer.h
                  include/VertexAdditionInteractor.h
                  include/VertexAdditionToPrimalInteractor.h
                  include/VertexSelectionInteractor.h
                  include/EdgeSelectionInteractor.h
                  include/EdgeAdditionInteractor.h
                  include/CellFeatureTableModel.h   
)
    
SET(TTTGUI_SRCS     src/tttmainwindow.cpp 
                    src/tttsqlnewprojectdialog.cpp 
                    src/tttsqlopenprojectdialog.cpp 
                    src/tttexplorer.cpp 
                    src/tttpreferences.cpp 
                    src/CellFeatureTableModel.cpp     
                    src/ProjectTableModel.cpp
                    src/VertexAdditionInteractor.cpp 
                    src/VertexSelectionInteractor.cpp 
                    src/VertexAdditionToPrimalInteractor.cpp 
                    src/EdgeSelectionInteractor.cpp 
                    src/EdgeAdditionInteractor.cpp
                    src/main.cpp
)

# this will run uic on .ui files:
include_directories( ${CMAKE_SOURCE_DIR}/Modules/IO/include/)
include_directories( ${CMAKE_SOURCE_DIR}/Modules/Core/include/)
include_directories( ${CMAKE_SOURCE_DIR}/Modules/Drawers/include/)
include_directories( ${CMAKE_SOURCE_DIR}/Modules/Commands/DataCasting/include/)
include_directories( ${CMAKE_SOURCE_DIR}/Modules/Commands/Plateness/include/)
include_directories( ${CMAKE_SOURCE_DIR}/Modules/Commands/Vertexness/include/)
include_directories( ${CMAKE_SOURCE_DIR}/Modules/Commands/ObjectnessMeasurementBase/include/)
include_directories( ${CMAKE_SOURCE_DIR}/Modules/Commands/AnisotropicDiffusion/include/)
include_directories( ${CMAKE_SOURCE_DIR}/Modules/Commands/VertexLocation/include/)
include_directories( ${CMAKE_SOURCE_DIR}/Modules/Commands/AdherensJunctionSegmentationDijkstra/include/ )
include_directories( ${CMAKE_SOURCE_DIR}/Modules/Commands/CellGraph/include/ )
include_directories( ${CMAKE_SOURCE_DIR}/Modules/Commands/Base/include/)

include_directories( ${CMAKE_SOURCE_DIR}/Modules/Commands/Tracking/include/)
include_directories( ${CMAKE_SOURCE_DIR}/Modules/Commands/Ellipses/include/)
include_directories( ${CMAKE_SOURCE_DIR}/Modules/Commands/ComputeDomains/include/ )
include_directories( ${CMAKE_SOURCE_DIR}/Modules/Commands/Tectonics/include/ )

include_directories( ${CMAKE_SOURCE_DIR}/Modules/Features/include/)
include_directories(./include)

qt5_wrap_ui(UI_HDRS ${TTTGUI_UIS} )
qt5_wrap_cpp(MOC_SRCS ${TTTGUI_MOCS})
add_executable(TTTGui ${TTTGUI_SRCS} ${UI_HDRS} ${MOC_SRCS} )
 
qt5_use_modules(TTTGui Widgets)
qt5_use_modules(TTTGui Sql)

target_link_libraries(TTTGui ${VTK_LIBRARIES} 
					         ${ITK_LIBRARIES} 
					         ${MYSQLCPPCONN_LIBRARY}
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
					         TTTIO
					         ${COIN_CBC_LIBRARIES}  
					         ${ZLIB_LIBRARIES} 
					         ${BZIP2_LIBRARIES} 
					         ${LAPACK_LIBRARIES}
					)
					
install(TARGETS TTTGui DESTINATION bin)