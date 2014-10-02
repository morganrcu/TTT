Project(SelectedCellsVolumeDrawing)
include_directories( ../../Commands/AdherensJunctionSegmentationDijkstra/include/)
include_directories( ../../Commands/Base/include/)
include_directories( ../../TrackVolume)

include_directories( ../../Drawers/include/)
include_directories( ../../Assesment/AssesmentBase/include/)

include_directories( ../../Core/include/)
include_directories( ../../IO/include/)
include_directories( ../../Features/include/)

add_executable(SelectedCellsVolumeDrawing source/SelectedCellsVolumeDrawing.cpp)
qt5_use_modules(SelectedCellsVolumeDrawing Sql)
target_link_libraries(SelectedCellsVolumeDrawing ${ITK_LIBRARIES} ${VTK_LIBRARIES} TTTIO TTTDrawers TTTAdherensJunctionSegmentationDijkstra TTTCore)
 
