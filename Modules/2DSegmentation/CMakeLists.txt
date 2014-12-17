Project(2DSegmentation)

include_directories( ../Commands/Base/include/)
include_directories( ../Commands/VertexLocation/include/)
include_directories( ../Commands/AdherensJunctionSegmentationDijkstra/include/)
include_directories( ../Commands/CellGraph/include/)
include_directories( ../Drawers/include/)
include_directories(../Features/include/)
include_directories( ../Core/include/)
include_directories( ../IO/include/)
#include_directories( ../../Interactors/include/)
#include_directories( ../AssesmentBase/include/)
include_directories( ./include/)

add_executable(2DSegmentation source/main.cpp)

target_link_libraries(2DSegmentation ${ITK_LIBRARIES} TTTDrawers jsoncpp)
 
qt5_use_modules(2DSegmentation Sql)