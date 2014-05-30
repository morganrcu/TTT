Project (TTTTrackAnnotation)

FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)

set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
add_definitions(${Qt5Widgets_DEFINITIONS})

# some .ui files sqlnewprojectdialog.ui sqlopenprojectdialog.ui
SET(TTTTrackAnnotation_UIS ttttrackannotation.ui 
    )

# another list, this time it includes all header files that should be treated with moc
SET( TTTTrackAnnotation_MOC_HDRS
  trackannotation.h
  )

SET(TTTTrackExplorer_SRCS
  trackannotation.cpp
   )
   

# this will run uic on .ui files:
include_directories( ../../Modules/Core/include/)
include_directories( ../../Modules/Drawers/include/)

include_directories( ../../Modules/Commands/DataCasting/include/)

include_directories( ../../Modules/Commands/Vertexness/include/)
include_directories( ../../Modules/Commands/Plateness/include/)
include_directories( ../../Modules/Commands/ObjectnessMeasurementBase/include/)

include_directories( ../../Modules/Commands/VertexLocation/include/)
include_directories( ../../Modules/IO/include/)


include_directories( ../../Modules/Commands/Base/include/)

include_directories( ../Features/include/)


qt5_wrap_ui(UI_HDRS ttttrackannotation.ui)

add_executable(TTTTrackAnnotation main.cpp ttttrackannotation.cpp ${UI_HDRS} MouseInteractorStyle.cpp)  
qt5_use_modules(TTTTrackAnnotation Widgets)
qt5_use_modules(TTTTrackAnnotation Sql)


target_link_libraries(TTTTrackAnnotation 
					${VTK_LIBRARIES} 
					${ITK_LIBRARIES} 

					TTTDrawers  
					TTTVertexness 
					TTTDataCasting 
					TTTCore
					TTTVertexLocation
					TTTIO
					mysqlcppconn
					
					
					)

