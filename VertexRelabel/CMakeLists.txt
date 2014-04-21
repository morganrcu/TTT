Project(VertexRelabel)

add_executable(VertexRelabel main.cpp)

target_link_libraries(VertexRelabel ${ITK_LIBRARIES} ${VTK_LIBRARIES})
 
