Project(SegmentationDrawing)
include_directories( ../../Commands/AdherensJunctionSegmentationDijkstra/include/)
include_directories( ../../Commands/Base/include/)

include_directories( ../../Drawers/include/)
include_directories( ../../Assesment/AssesmentBase/include/)

include_directories( ../../Core/include/)
include_directories( ../../IO/include/)
include_directories( ../../Features/include/)



add_executable(SegmentationDrawing source/SegmentationAssesment.cpp)

target_link_libraries(SegmentationDrawing ${ITK_LIBRARIES} ${VTK_LIBRARIES} TTTIO TTTDrawers TTTAdherensJunctionSegmentationDijkstra mysqlcppconn)
 
