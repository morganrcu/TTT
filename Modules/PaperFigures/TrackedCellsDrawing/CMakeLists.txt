Project(TrackedCellsDrawing)
include_directories( ../../Commands/AdherensJunctionSegmentationDijkstra/include/)
include_directories( ../../Commands/Base/include/)

include_directories( ../../Drawers/include/)
include_directories( ../../Assesment/AssesmentBase/include/)

include_directories( ../../Core/include/)
include_directories( ../../IO/include/)
include_directories( ../../Features/include/)



add_executable(TrackedCellsDrawing source/TrackedCellsDrawing.cpp)
qt5_use_modules(TrackedCellsDrawing Sql)
target_link_libraries(TrackedCellsDrawing ${ITK_LIBRARIES} ${VTK_LIBRARIES} TTTIO TTTDrawers TTTAdherensJunctionSegmentationDijkstra )
 
