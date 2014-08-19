Project(TTTIO)

include_directories( include)
include_directories(../Core/include/)
include_directories(../Features/include/)

#add_executable(migratemysqltojson source/migratemysqltojson.cpp )

#add_executable(testnewprojectapi source/testnewprojectapi.cpp)
add_library(TTTIO source/mysqltissuetrackingproject2.cpp
                  source/jsontissuetrackingproject2.cpp
)

#target_link_libraries(TTTIO mysqlcppconn jsoncpp)
#target_link_libraries(testnewprojectapi TTTIO jsoncpp)

#target_link_libraries(migratemysqltojson TTTIO ${ITK_LIBRARIES} jsoncpp mysqlcppconn)
