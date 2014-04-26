Project(TTTTracking)

include_directories(include)
include_directories(../Base/include/)
include_directories(../../Core/include/)
include_directories(../../Features/include/)
include_directories( include)
find_package( ZLIB REQUIRED )
#INCLUDE_DIRECTORIES(/home/morgan/Development/solvers/coin-Cbc/include/coin)
#link_directories(/home/morgan/Development/solvers/coin-Cbc/lib)

set(TTTTracking_SRCS source/TrackingCommand.cpp 
                     #source/giaaCellTracker.cpp
                     source/tttMinCostMaxFlowCellTracker.cpp)

add_executable(mincostmaxflowcelltrackertest source/mincostmaxflowcelltrackertest.cpp source/tttMinCostMaxFlowCellTracker.cpp)

target_link_libraries(mincostmaxflowcelltrackertest ${ITK_LIBRARIES} Cbc CbcSolver Cgl Clp CoinUtils bz2   OsiClp Osi ${ZLIB_LIBRARIES} lapack)
 
add_library(TTTTracking ${TTTTracking_SRCS})