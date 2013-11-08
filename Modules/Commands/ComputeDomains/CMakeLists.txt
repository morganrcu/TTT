Project(TTTComputeDomains)

include_directories(include)
include_directories(../Base/include/)
include_directories(../../Core/include/)

include_directories( include)

set(TTTComputeDomains_SRCS source/ComputeDomainsCommand.cpp)

add_library(TTTComputeDomains ${TTTComputeDomains_SRCS})