Project(TTTAdherensJunctionSegmentation)

include_directories( ./include/)
include_directories(../Base/include/)
include_directories(../../Core/include/)
set(TTTAdherensJunctionSegmentation_SRCS source/AdherensJunctionSegmentationCommand.cpp)

add_library(TTTAdherensJunctionSegmentation ${TTTAdherensJunctionSegmentation_SRCS})

