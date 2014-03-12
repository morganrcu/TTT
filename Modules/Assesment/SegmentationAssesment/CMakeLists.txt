Project(SegmentationAssesment)
include_directories( ../../Commands/AdherensJunctionSegmentationDijkstra/include/)
include_directories( ../../Commands/Base/include/)

include_directories( ../../Drawers/include/)
include_directories( ../AssesmentBase/include/)
include_directories( ../../Core/include/)
include_directories( ../../IO/include/)



add_executable(SegmentationAssesment source/SegmentationAssesment.cpp)

target_link_libraries(SegmentationAssesment ${ITK_LIBRARIES} TTTDrawers TTTAdherensJunctionSegmentationDijkstra mysqlcppconn)
 
