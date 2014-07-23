Project (TTTDeconvolution)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

  
# this will run uic on .ui files:

include_directories(./include/)
add_executable(TTTDeconvolution source/deconvolution.cpp)  


target_link_libraries(TTTDeconvolution 
					${VTK_LIBRARIES} 
					${ITK_LIBRARIES} 			
					
					)

