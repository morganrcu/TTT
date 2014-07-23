Project (TTTRemoveOverlap)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

  
# this will run uic on .ui files:


add_executable(TTTRemoveOverlap source/removeoverlap.cpp)  


target_link_libraries(TTTRemoveOverlap 
					${VTK_LIBRARIES} 
					${ITK_LIBRARIES} 			
					
					)

