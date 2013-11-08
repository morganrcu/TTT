Project(TTTCore)
include_directories(include)

SET(TTTCORE_SRCS source/tttDomainStrainRates.cpp source/tttDescriptionDataTypes.cpp)

ADD_LIBRARY(TTTCore ${TTTCORE_SRCS})