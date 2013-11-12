Project(TTTGUI)	

include_directories( ./include/)
include_directories( ../IO/include/)
include_directories( ../Core/include/)
include_directories( ../Drawers/include/)
include_directories( ../Interactors/include/)
include_directories( ../Tectonics/include/)
include_directories( ../Commands/Tracking/include/)
include_directories( ../Commands/SurfaceSegmentation/include/)
include_directories( ../Commands/AnisotropicDiffusion/include/)
include_directories( ../Commands/Plateness/include/)
include_directories( ../Commands/Vertexness/include/)
include_directories( ../Commands/VertexLocation/include/)
include_directories( ../Commands/AdherensJunctionSegmentation/include/)
include_directories( ../Commands/CellGraph/include/)
include_directories( ../Commands/Ellipses/include/)
include_directories( ../Commands/DataCasting/include/)
include_directories( ../Commands/ComputeDomains/include/)
include_directories( ../Commands/Tectonics/include/)
include_directories( ../Commands/Base/include/)
include_directories( ../Commands/ObjectnessMeasurementBase/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# some .ui files sqlnewprojectdialog.ui sqlopenprojectdialog.ui
SET(TTTGUI_UIS tissuetracker.ui 
    )

# another list, this time it includes all header files that should be treated with moc
SET( TTTGUI_MOC_HDRS
    include/tissuetracker.h
    include/sqlnewprojectdialog.h
    include/sqlopenprojectdialog.h
    )

SET(TTTGUI_SRCS
    source/tissuetracker.cpp
    source/sqlnewprojectdialog.cpp
    source/sqlopenprojectdialog.cpp
    source/main.cpp
    )

# this will run uic on .ui files:

QT4_WRAP_UI(TTTGUI_UI_HDRS tissuetracker.ui  )
QT4_WRAP_CPP(TTTGUI_MOC_SRCS ${TTTGUI_MOC_HDRS} )

#include_directories( ${CMAKE_BINARY_DIR} .)
# and finally this will run moc:
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

ADD_EXECUTABLE( TTTGUI ${TTTGUI_SRCS} ${TTTGUI_MOC_SRCS} ${TTGUI_UI_HDRS} )

TARGET_LINK_LIBRARIES( TTTGUI 
    TTTDataCasting 
    TTTComputeDomains 
    TTTAnisotropicDiffusion 
    TTTTracking 
    TTTAdherensJunctionSegmentation 
    TTTPlateness 
    TTTCore
    TTTVertexLocation
    TTTDrawers
    TTTInteractors
    TTTVertexness
    TTTTectonics
    TTTCellGraph
    TTTSurfaceSegmentation
    ${ITK_LIBRARIES} 
    ${VTK_LIBRARIES} 
    mysqlcppconn 
    ${QT_LIBRARIES} 
    ${Boost_LIBRARIES}
)

INSTALL(TARGETS TTTGUI DESTINATION bin )