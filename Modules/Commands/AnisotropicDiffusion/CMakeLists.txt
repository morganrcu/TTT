Project(TTTAnisotropicDiffusion)

include_directories(include)
include_directories(../Base/include/)
include_directories(../ObjectnessMeasurementBase/include/)
include_directories(../Plateness/include/)
include_directories(../../Core/include/)

set(TTTAnisotropicDiffusion_SRCS source/AnisotropicDiffusionCommand.cpp)

add_library(TTTAnisotropicDiffusion ${TTTAnisotropicDiffusion_SRCS})

add_executable(TTTAnisotropicDiffusionTest test/AnisotropicDiffusionCommandTest.cpp)

target_link_libraries(TTTAnisotropicDiffusionTest ${ITK_LIBRARIES}  TTTAnisotropicDiffusion )