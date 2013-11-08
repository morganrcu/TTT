Project(TTTAdherensJunction)

include_directories( ./include/)
include_directories(../Base/include/)
include_directories(../../Core/include/)
set(TTTAdherensJunction_SRCS source/AdherensJunctionSegmentationCommand.cpp)

add_library(TTTAdherensJunctions ${TTTAdherensJunction_SRCS})

