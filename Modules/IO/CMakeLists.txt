Project(TTTIO)

include_directories( include)
include_directories(../Core/include/)

add_library(TTTIO source/mysqltissuetrackingproject.cpp)

target_link_libraries(TTTIO mysqlcppconn)