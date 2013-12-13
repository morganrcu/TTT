Project(VertexLocationAssesment)

include_directories( ../../Commands/Base/include/)
include_directories( ../../Commands/VertexLocation/include/)
include_directories( ../../Drawers/include/)
include_directories( ../../Core/include/)
include_directories( ../../Interactors/include/)
include_directories( ../AssesmentBase/include/)


add_executable(VertexLocationAssesment source/VertexLocationAssesment.cpp)

target_link_libraries(VertexLocationAssesment ${ITK_LIBRARIES} TTTDrawers TTTInteractors)
 
