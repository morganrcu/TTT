Project(TTTDataCasting)

include_directories(include)
include_directories(../Base/include/)
include_directories(../../Core/include/)

include_directories( include)

set(TTTDataCasting_SRCS source/DataCastingCommand.cpp)

add_library(TTTDataCasting ${TTTDataCasting_SRCS})