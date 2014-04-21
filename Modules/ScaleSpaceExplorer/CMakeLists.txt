Project (TTTScaleSpaceExplorer)

FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)

set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
add_definitions(${Qt5Widgets_DEFINITIONS})

# some .ui files sqlnewprojectdialog.ui sqlopenprojectdialog.ui
SET(TTTScaleSpaceExplorer_UIS scalespaceexplorer.ui 
    )

# another list, this time it includes all header files that should be treated with moc
SET( TTTScaleSpaceExplorer_MOC_HDRS
  scalespaceexplorer.h
  )

SET(TTTScaleSpaceExplorer_SRCS
  scalespaceexplorer.cpp
   )
   

# this will run uic on .ui files:
include_directories( ../../Modules/Core/include/)
include_directories( ../../Modules/Drawers/include/)

include_directories( ../../Modules/Commands/DataCasting/include/)

include_directories( ../../Modules/Commands/Vertexness/include/)
include_directories( ../../Modules/Commands/Plateness/include/)
include_directories( ../../Modules/Commands/ObjectnessMeasurementBase/include/)

include_directories( ../../Modules/Commands/VertexLocation/include/)


include_directories( ../../Modules/Commands/Base/include/)


qt5_wrap_ui(UI_HDRS scalespaceexplorer.ui)

add_executable(TTTScaleSpaceExplorer main.cpp scalespaceexplorer.cpp ${UI_HDRS})  
qt5_use_modules(TTTScaleSpaceExplorer Widgets)
qt5_use_modules(TTTScaleSpaceExplorer Sql)

target_link_libraries(TTTScaleSpaceExplorer 
					${VTK_LIBRARIES} 
					${ITK_LIBRARIES} 

					TTTDrawers  
					TTTVertexness 
					TTTDataCasting 
					 
					TTTVertexLocation
					
					
					
					)

