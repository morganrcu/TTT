Project(TrackingOptimizing)
include_directories( ../../Commands/AdherensJunctionSegmentationDijkstra/include/)
include_directories( ../../Commands/Base/include/)
include_directories( ../../Commands/Tracking/include/)
include_directories( ../../TrackVolume/)

include_directories( ../../Drawers/include/)
include_directories( ../AssesmentBase/include/)
include_directories( ../TrackingAssesmentCommon/include/)
include_directories( ../../Core/include/)
include_directories( ../../IO/include/)
include_directories( ../../Features/include/)



add_executable(TrackingOptimizing source/main.cpp)

qt5_use_modules(TrackingOptimizing Sql)
target_link_libraries(TrackingOptimizing ${VTK_LIBRARIES}
                                #vtkjsoncpp
                                #vtkGUISupportQt
                                #vtkGUISupportQtOpenGL
                                 
					         ${ITK_LIBRARIES} 
					         
					         TTTDrawers 
			         
				         	jsoncpp
					         ${COIN_CBC_LIBRARIES}  
					         ${ZLIB_LIBRARIES}
					          
					         ${BZIP2_LIBRARIES} 
					         ${LAPACK_LIBRARIES} 
)
 
