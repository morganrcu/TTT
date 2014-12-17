Project(TTTTracking)

include_directories(include)
include_directories(../Base/include/)
include_directories(../CellGraph/include/)
include_directories(../../Core/include/)
include_directories(../../Features/include/)
include_directories( include)

set(TTTTracking_SRCS source/TrackingCommand.cpp 
                     source/TrackInitializationCommand.cpp 
                     source/MinCostMaxFlowTrackAssociationCommand.cpp
                      )

#add_executable(mincostmaxflowcelltrackertest source/mincostmaxflowcelltrackertest.cpp source/tttMinCostMaxFlowCellTracker.cpp)

#target_link_libraries(mincostmaxflowcelltrackertest ${ITK_LIBRARIES} ${COIN_CBC_LIBRARIES} ${ZLIB_LIBRARIES} lapack TTTCellGraph)
 
#add_library(TTTTracking ${TTTTracking_SRCS})