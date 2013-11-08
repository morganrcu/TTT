Project(TTTSurfaceSegmentation)

include_directories(include)
include_directories(../Base/include/)
include_directories(../../Core/include/)

include_directories( include)

set(TTTSurfaceSegmentation_SRCS source/SurfaceSegmentationCommand.cpp)

add_library(TTTSurfaceSegmentation ${TTTSurfaceSegmentation_SRCS})
