Project(TTTDrawers)

include_directories(include)

include_directories(../Core/include/)

set(TTTDrawers_SRCS 
	source/DiffusedImageDrawer.cpp
	source/DomainStrainRatesDrawer.cpp
	source/Drawer.cpp
	source/DualGraphDrawer.cpp
	source/EllipseDrawer.cpp
	source/GreetingsDrawer.cpp
	source/MotionVectorDrawer.cpp
	source/PlatenessImageDrawer.cpp
	source/PrimalGraphDrawer.cpp
	source/RawImageDrawer.cpp
	source/TrackingDrawer.cpp
	source/VertexLocationsDrawer.cpp
	source/VertexnessImageDrawer.cpp
	)
add_library(TTTDrawers ${TTTDrawers_SRCS})