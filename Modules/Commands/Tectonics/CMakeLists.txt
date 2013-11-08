Project(TTTTectonics)

include_directories(include)
include_directories(../Base/include/)
include_directories(../../Core/include/)

include_directories( include)

set(TTTTectonics_SRCS source/TectonicsCommand.cpp source/tectonics.cpp)

add_library(TTTTectonics ${TTTTectonics_SRCS})
target_link_libraries(TTTTectonics TTTCore)