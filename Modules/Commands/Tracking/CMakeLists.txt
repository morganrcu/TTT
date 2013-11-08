Project(TTTTracking)

include_directories(include)
include_directories(../Base/include/)
include_directories(../../Core/include/)

include_directories( include)

set(TTTTracking_SRCS source/TrackingCommand.cpp)

#add_library(TTTTracking_SRCS ${TTTTracking_SRCS})