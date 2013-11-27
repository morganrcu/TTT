Project(TTTPlateness)

include_directories(include)
include_directories(../ObjectnessMeasurementBase/include)
include_directories(../Base/include/)
include_directories(../../Core/include/)



set(TTTPlateness_SRCS source/PlatenessCommand.cpp)

add_library(TTTPlateness ${TTTPlateness_SRCS})

add_executable(TTTPlatenessCommandTest test/PlatenessCommandTest.cpp)

target_link_libraries(TTTPlatenessCommandTest ${ITK_LIBRARIES} TTTPlateness)