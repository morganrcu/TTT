Project(TTTVertexLocation)

include_directories(include)
include_directories(../Base/include/)
include_directories(../../Core/include/)

include_directories( include)

set(TTTVertexLocation_SRCS source/VertexLocationCommand.cpp)

add_library(TTTVertexLocation ${TTTVertexLocation_SRCS})

add_executable(TTTVertexLocationTest test/VertexLocationCommandTest.cpp)

target_link_libraries(TTTVertexLocationTest ${ITK_LIBRARIES}  TTTVertexLocation )