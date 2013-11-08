Project(TTTPlatenessCommand)

include_directories(include)
include_directories(../Base/include/)
include_directories(../../Core/include/)

include_directories( include)

set(TTTPlatenessCommand_SRCS source/PlatenessCommand.cpp)

#add_library(TTTPlatenessCommand ${TTTPlatenessCommand_SRCS})