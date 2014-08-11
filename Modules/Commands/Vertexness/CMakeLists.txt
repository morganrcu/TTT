Project(TTTVertexness)

include_directories(include)
include_directories(../Base/include/)
include_directories(../ObjectnessMeasurementBase/include/)
include_directories(../../Core/include/)


set(TTTVertexness_SRCS source/VertexnessCommand.cpp source/PyramidVertexnessCommand.cpp)

add_library(TTTVertexness ${TTTVertexness_SRCS})

add_executable(TTTVertexnessCommandTest test/VertexnessCommandTest.cpp)

target_link_libraries(TTTVertexnessCommandTest ${ITK_LIBRARIES} TTTVertexness)