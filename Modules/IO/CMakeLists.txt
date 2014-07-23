Project(TTTIO)

include_directories( include)
include_directories(../Core/include/)
include_directories(../Features/include/)

add_library(TTTIO source/mysqltissuetrackingproject.cpp
                  source/jsontissuetrackingproject.cpp)

target_link_libraries(TTTIO mysqlcppconn)