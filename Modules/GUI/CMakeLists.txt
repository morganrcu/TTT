Project(TTTGUI)	

include_directories( ./include/)
include_directories( ${CMAKE_BINARY_DIR} .)

# some .ui files
SET( TTTGUI_UIS
    ui/tissuetracker.ui
    ui/sqlnewprojectdialog.ui
    ui/sqlopenprojectdialog.ui
    )

# another list, this time it includes all header files that should be treated with moc
SET( TTTGUI_MOC_HDRS
    include/tissuetracker.h
    include/sqlnewprojectdialog.h
    include/sqlopenprojectdialog.h
    )

SET( TTTGUI_SRCS
    source/tissuetracker.cpp
    source/sqlnewprojectdialog.cpp
    source/sqlopenprojectdialog.cpp
    source/main.cpp
    )
    
# this will run uic on .ui files:
QT4_WRAP_UI( TTTGUI_UI_HDRS ${TTTGUI_UIS} )
  
# and finally this will run moc:
QT4_WRAP_CPP(TTTGUIi_MOC_SRCS ${TTTGUI_MOC_HDRS} )

ADD_EXECUTABLE( TTTGUI ${TTTGUI_SRCS} ${TTTGUI_MOC_SRCS} ${TTGUI_UI_HDRS} )