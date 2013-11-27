Project(TTTCLAHE)

include_directories( include)

include_directories( ../Base/include)

set(TTTCLAHE_SRCS source/CLAHECommand.cpp)

add_library(TTTCLAHE ${TTTCLAHE_SRCS})


add_executable(TTTCLAHETest test/CLAHECommandTest.cpp)

target_link_libraries(TTTCLAHETest ${ITK_LIBRARIES}  TTTCLAHE )