Project(VertexLocationDrawing)

include_directories( ../../Commands/Base/include/)
include_directories( ../../Commands/VertexLocation/include/)
include_directories( ../../Drawers/include/)
include_directories( ../../Core/include/)
include_directories( ../..//IO/include/)
include_directories( ../../Interactors/include/)
include_directories( ../../Assesment/AssesmentBase/include/)


add_executable(VertexLocationDrawing source/VertexLocationDrawing.cpp)

target_link_libraries(VertexLocationDrawing ${ITK_LIBRARIES} ${VTK_LIBRARIES} TTTDrawers TTTInteractors)
 
