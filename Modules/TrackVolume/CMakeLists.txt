Project(TTTTrackVolume)
include_directories( ../../Commands/AdherensJunctionSegmentationDijkstra/include/)
include_directories( ../../Commands/Base/include/)

include_directories( ../../Drawers/include/)
include_directories( ../AssesmentBase/include/)
include_directories( ../Core/include/)
include_directories( ../IO/include/)
include_directories( ../Features/include/)



add_executable(TTTTrackVolume main.cpp)

target_link_libraries(TTTTrackVolume ${ITK_LIBRARIES} ${VTK_LIBRARIES} TTTDrawers TTTCore TTTIO mysqlcppconn)
 