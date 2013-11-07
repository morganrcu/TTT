 # with SET() command you can change variables or define new ones
  # here we define TTGui_SRCS variable that contains a list of all .cpp files
  # note that we don't need \ at the end of line

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(./Drawers)
INCLUDE_DIRECTORIES(./Interactors)
INCLUDE_DIRECTORIES(./Commands)

SET( TTGui_SRCS
    #tissuetrackingproject.cpp
    tissuetracker.cpp
    sqlnewprojectdialog.cpp
    sqlopenprojectdialog.cpp
    main.cpp
    #cluster.c 
    tectonics.cpp
    
    Drawers/Drawer.cpp
    Drawers/GreetingsDrawer.cpp
    Drawers/RawImageDrawer.cpp
    Drawers/DiffusedImageDrawer.cpp
    Drawers/PlatenessImageDrawer.cpp
    Drawers/VertexnessImageDrawer.cpp
    Drawers/VertexLocationsDrawer.cpp
    Drawers/PrimalGraphDrawer.cpp
    Drawers/DualGraphDrawer.cpp
    Drawers/TrackingDrawer.cpp
    Drawers/MotionVectorDrawer.cpp
    Drawers/EllipseDrawer.cpp
    Drawers/DomainStrainRatesDrawer.cpp
    
	Interactors/PrimalGraphInteractor.cpp
	Interactors/PrimalGraphStandardInteractor.cpp
    Interactors/PrimalGraphInsertionInteractor.cpp
    Interactors/PrimalGraphMoveInteractor.cpp
    
    Commands/DataCastingCommand.cpp
    Commands/SurfaceSegmentationCommand.cpp
    Commands/AnisotropicDiffusionCommand.cpp
    Commands/PlatenessCommand.cpp
    Commands/VertexnessCommand.cpp
    Commands/VertexLocationCommand.cpp
    Commands/VinodthSegmentationCommand.cpp
    Commands/DualGraphCommand.cpp
    Commands/TrackingCommand.cpp
    Commands/ComputeDomainsCommand.cpp
    Commands/TectonicsCommand.cpp
    #Commands/EllipseCommand.cpp
  )

  # another list, this time it includes all header files that should be treated with moc
  SET( TTGui_MOC_HDRS
     tissuetracker.h
     sqlnewprojectdialog.h
     sqlopenprojectdialog.h
     tectonics.h
  )

  # some .ui files
  SET( TTGui_UIS
     tissuetracker.ui
     sqlnewprojectdialog.ui
     sqlopenprojectdialog.ui
  )

  # and finally an resource file
  #SET( TTGui_RCS
  #     ./src/rc/TTGui.qrc
  #)

  # this command will generate rules that will run rcc on all files from TTGui_RCS
  # in result TTGui_RC_SRCS variable will contain paths to files produced by rcc
  #QT4_ADD_RESOURCES( TTGui_RC_SRCS ${TTGui_RCS} )

  # this will run uic on .ui files:
  QT4_WRAP_UI( TTGui_UI_HDRS ${TTGui_UIS} )
  
  # and finally this will run moc:
  QT4_WRAP_CPP( TTGui_MOC_SRCS ${TTGui_MOC_HDRS} )

  # we need this to be able to include headers produced by uic in our code
  # (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
  INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} .) 
   
  # here we instruct CMake to build "TTGui" executable from all of the source files
  ADD_EXECUTABLE( TTGui ${TTGui_SRCS} ${TTGui_MOC_SRCS} ${TTGui_RC_SRCS} ${TTGui_UI_HDRS} )

  # last thing we have to do is to tell CMake what libraries our executable needs,
  # luckily FIND_PACKAGE prepared QT_LIBRARIES variable for us:
  TARGET_LINK_LIBRARIES( TTGui mysqlcppconn ${QT_LIBRARIES} ${VTK_LIBRARIES} ${ITK_LIBRARIES} ${Boost_LIBRARIES} TissueSegmentation fftw3)
