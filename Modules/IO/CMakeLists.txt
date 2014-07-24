Project(TTTIO)

include_directories( include)
include_directories(../Core/include/)
include_directories(../Features/include/)
include_directories(/usr/include/jsoncpp/)
add_executable(migratemysqltojson source/migratemysqltojson.cpp)
add_library(TTTIO source/mysqltissuetrackingproject.cpp
                  source/jsontissuetrackingproject.cpp)

target_link_libraries(TTTIO mysqlcppconn)
target_link_libraries(migratemysqltojson TTTIO ${ITK_LIBRARIES} jsoncpp)