Project(TTTIO)

include_directories( include)
include_directories(../Core/include/)
include_directories(../Features/include/)
include_directories(/usr/include/jsoncpp/)
add_executable(migratemysqltojson source/migratemysqltojson.cpp source/mysqltissuetrackingproject.cpp)

#add_executable(testnewprojectapi source/testnewprojectapi.cpp)
add_library(TTTIO source/mysqltissuetrackingproject.cpp
                  source/jsontissuetrackingproject.cpp
                                )

target_link_libraries(TTTIO mysqlcppconn jsoncpp)
#target_link_libraries(testnewprojectapi TTTIO jsoncpp)

target_link_libraries(migratemysqltojson ${ITK_LIBRARIES} jsoncpp mysqlcppconn)
