Project(TTTSurface)

include_directories(include)
include_directories(../Base/include/)
include_directories(../../Core/include/)
include_directories(../ObjectnessMeasurementBase/include/)
include_directories(../Vertexness/include/)

include_directories( include)

set(TTTSurface_SRCS source/SurfaceCommand.cpp)

add_library(TTTSurface ${TTTSurface_SRCS})

add_executable(TTTSurfaceCommandTest test/SurfaceCommandTest.cpp)

target_link_libraries(TTTSurfaceCommandTest ${ITK_LIBRARIES} TTTSurface)