#-------------------------------------------------
#
# Project created by QtCreator 2011-10-12T11:24:56
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TissueTracker
TEMPLATE = app


SOURCES += source/main.cpp\
        source/tissuetracker.cpp \
    source/tissuetrackingproject.cpp \
    source/newprojectdialog.cpp \
    source/sqlopenprojectdialog.cpp

HEADERS  += include/tissuetracker.h \
    include/tissuetrackingproject.h \
    include/newprojectdialog.h \
    include/sqlopenprojectdialog.h

FORMS    += tissuetracker.ui \
    sqlnewprojectdialog.ui \
    sqlopenprojectdialog.ui

INCLUDEPATH+=/usr/local/include/vtk-6.0 /usr/local/include/ITK-4.4 ./include

#LIBS += -L../../../tissuetracking-bin/ -lTissueTrackerFacade -lITKCommon-4.1 -L/usr/local/lib/vtk-5.8 -lvtkCommon -lvtkGraphics -lvtkRendering -lvtkVolumeRendering -lITKVTK-4.1 -lITKIOImageBase-4.1 -litkvnl-4.1 -lITKIOJPEG-4.1 -lITKIOGDCM-4.1 -lITKIOBMP-4.1 -lITKIOLSM-4.1 -lITKIOPNG-4.1 -lITKIOVTK-4.1 -lITKIOStimulate-4.1 -lITKIOBioRad-4.1 -lITKIOMeta-4.1 -lITKIONIFTI-4.1 -lITKIONRRD-4.1 -lITKIOGIPL-4.1 -lITKIOHDF5-4.1 -lQVTK
#LIBS += -lboost_serialization
#LIBS += -L/usr/local/lib/vtk-5.8 -lvtkalglib -lvtkDICOMParser -lvtkFiltering  -lvtkGenericFiltering -lvtkhdf5 -lvtkInfovis -lvtklibxml2 -lvtkNetCDF_cxx -lvtkproj4 -lvtksys     -lvtkViews           -lvtkzlib -lvtkCharts -lvtkexoIIc      -lvtkfreetype  -lvtkGeovis           -lvtkHybrid  -lvtkIO      -lvtkmetaio  -lvtkParallel   -lvtkRendering -lvtktiff    -lvtkVolumeRendering -lvtkCommon -lvtkexpat       -lvtkftgl      -lvtkGraphics         -lvtkImaging -lvtkjpeg    -lvtkNetCDF  -lvtkpng        -lvtksqlite    -lvtkverdict -lvtkWidgets -lvtkIO -lvtkRendering -lvtkVolumeRendering -lvtksys -lQVTK
#LIBS += -L/usr/local/lib/ -litkgdcmCommon-4.1 -litkgdcmIOD-4.1 -litkgdcmjpeg8-4.1 -litkhdf5-4.1  -litkNetlibSlatec-4.1 -litksys-4.1 -litkv3p_netlib-4.1 -litkvnl_algo-4.1 -litkgdcmDICT-4.1   -litkgdcmjpeg12-4.1 -litkgdcmMSFF-4.1  -litkhdf5_cpp-4.1 -litkopenjpeg-4.1     -litktiff-4.1     -litkvcl-4.1        -litkzlib-4.1 -litkgdcmDSED-4.1   -litkgdcmjpeg16-4.1 -litkgdcmuuid-4.1  -litkjpeg-4.1     -litkpng-4.1          -litkv3p_lsqr-4.1 -litkvnl-4.1 -lITKBiasCorrection-4.1 -lITKgiftiio-4.1  -lITKIOHDF5-4.1      -lITKIONIFTI-4.1          -lITKIOTransformBase-4.1          -lITKLabelMap-4.1     -lITKPath-4.1           -lITKVideoIO-4.1 -lITKBioCell-4.1        -lITKIOBioRad-4.1 -lITKIOImageBase-4.1 -lITKIONRRD-4.1           -lITKIOTransformHDF5-4.1          -lITKMesh-4.1         -lITKPolynomials-4.1    -lITKVNLInstantiation-4.1 -lITKCommon-4.1         -lITKIOBMP-4.1    -lITKIOIPL-4.1       -lITKIOPNG-4.1            -lITKIOTransformInsightLegacy-4.1 -lITKMetaIO-4.1       -lITKQuadEdgeMesh-4.1   -lITKVTK-4.1 -lITKDeprecated-4.1     -lITKIOCSV-4.1    -lITKIOJPEG-4.1      -lITKIOSiemens-4.1        -lITKIOTransformMatlab-4.1        -lITKniftiio-4.1      -lITKReview-4.1         -lITKWatersheds-4.1 -lITKDICOMParser-4.1    -lITKIOGDCM-4.1   -lITKIOLSM-4.1       -lITKIOSpatialObjects-4.1 -lITKIOVTK-4.1                    -lITKNrrdIO-4.1       -lITKSpatialObjects-4.1 -lITKznz-4.1 -lITKEXPAT-4.1          -lITKIOGE-4.1     -lITKIOMesh-4.1      -lITKIOStimulate-4.1      -lITKIOXML-4.1                    -lITKOptimizers-4.1   -lITKStatistics-4.1 -lITKFEM-4.1            -lITKIOGIPL-4.1   -lITKIOMeta-4.1      -lITKIOTIFF-4.1           -lITKKLMRegionGrowing-4.1         -lITKOptimizersv4-4.1 -lITKVideoCore-4.1 -lITKznz-4.1 -litkzlib-4.1
