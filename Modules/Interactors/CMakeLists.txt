Project(TTTInteractors)

include_directories(include)
include_directories(../Drawers/include)
include_directories(../Core/include)


SET(TTTInteractors_SRCS 
    source/PrimalGraphInsertionInteractor.cpp
    source/PrimalGraphStandardInteractor.cpp
    source/PrimalGraphMoveInteractor.cpp
    source/PrimalGraphInteractor.cpp
    
)
ADD_LIBRARY(TTTInteractors ${TTTInteractors_SRCS})