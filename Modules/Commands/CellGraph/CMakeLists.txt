Project(TTTCellGraph)

include_directories(include)
include_directories(../Base/include/)
include_directories(../../Core/include/)
include_directories(../../Features/include/)

include_directories( include)

set(TTTCellGraph_SRCS source/CellGraphCommand.cpp)

add_library(TTTCellGraph ${TTTCellGraph_SRCS})