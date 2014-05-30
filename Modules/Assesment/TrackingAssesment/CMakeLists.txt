Project(TrackingAssesment)
include_directories( ../../Commands/AdherensJunctionSegmentationDijkstra/include/)
include_directories( ../../Commands/Base/include/)
include_directories( ../../Commands/Tracking/include/)
include_directories( ../../TrackVolume/)

include_directories( ../../Drawers/include/)
include_directories( ../AssesmentBase/include/)
include_directories( ../../Core/include/)
include_directories( ../../IO/include/)
include_directories( ../../Features/include/)



add_executable(TrackingAssesment source/main.cpp)

target_link_libraries(TrackingAssesment ${ITK_LIBRARIES} ${VTK_LIBRARIES} mysqlcppconn TTTCore TTTTracking 			TTTIO		Cbc CbcSolver Cgl Clp CoinUtils bz2   OsiClp Osi ${ZLIB_LIBRARIES} lapack
)
 
