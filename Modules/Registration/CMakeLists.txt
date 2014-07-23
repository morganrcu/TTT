Project (TTTRegistration)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

  
# this will run uic on .ui files:


add_executable(TTTRigidRegistration source/rigidregistration.cpp)  


target_link_libraries(TTTRigidRegistration 
					${VTK_LIBRARIES} 
					${ITK_LIBRARIES} 			
					
					)

