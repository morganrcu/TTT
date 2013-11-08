Project(TTTPlateness)

include_directories(include)
include_directories(../Base/include/)
include_directories(../../Core/include/)

include_directories( include)

set(TTTPlateness_SRCS source/PlatenessCommand.cpp)

add_library(TTTPlateness ${TTTPlateness_SRCS})