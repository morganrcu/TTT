Project(TTTVertexLocation)

include_directories(include)
include_directories(../Base/include/)
include_directories(../../Core/include/)

include_directories( include)

set(TTTVertexLocation_SRCS source/VertexLocationCommand.cpp)

add_library(TTTVertexLocation ${TTTVertexLocation_SRCS})