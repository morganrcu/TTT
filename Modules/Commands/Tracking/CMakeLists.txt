Project(TTTTracking)

include_directories(include)
include_directories(../Base/include/)
include_directories(../../Core/include/)

include_directories( include)

set(TTTTracking_SRCS source/TrackingCommand.cpp source/giaaCellTracker.cpp)

add_library(TTTTracking ${TTTTracking_SRCS})