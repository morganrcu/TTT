Project(TrackAndDraw)
include_directories( ../Commands/AdherensJunctionSegmentationDijkstra/include/)
include_directories( ../Commands/Base/include/)
include_directories( ../Commands/Tracking/include/)
include_directories( ../TrackVolume/)


include_directories( ../../Drawers/include/)
include_directories( ../AssesmentBase/include/)
include_directories( ../TrackingAssesmentCommon/include/)
include_directories( ../Core/include/)
include_directories( ../IO/include/)
include_directories( ../Features/include/)

add_executable(TrackAndDraw main.cpp)

qt5_use_modules(TrackAndDraw Sql)

target_link_libraries(TrackAndDraw ${VTK_LIBRARIES}
                                                                 
					         ${ITK_LIBRARIES} 
					         
					         TTTDrawers 
					         TTTPlateness 
					         TTTVertexness 
					         TTTDataCasting 
					         TTTAnisotropicDiffusion 
					         TTTVertexLocation
					         TTTAdherensJunctionSegmentationDijkstra
					         TTTCellGraph
					         TTTTracking
					         TTTTectonics
					         
					         TTTComputeDomains
					         TTTIO
					         
					         ${COIN_CBC_LIBRARIES}  
					         ${ZLIB_LIBRARIES}
					          
					         ${BZIP2_LIBRARIES} 
					         ${LAPACK_LIBRARIES} 
)
 
