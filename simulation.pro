######################################################################
# Automatically generated by qmake (2.01a) ?? ???. 20 18:42:55 2013
######################################################################

TEMPLATE = app
TARGET = bin/simulation
DEPENDPATH += . include src
INCLUDEPATH += . include
OBJECTS_DIR = tmp
MOC_DIR     = tmp

# Input
HEADERS += include/Constants.h \
           include/DrawWidget.h \
           #include/DrawWidgetMW.h \
           include/FileExx.h \
           include/GraphData.h \
           include/MainWindow.h \
           include/ModelData.h \
           include/ShuffleIndexes.h \
           include/SimulationWorker.h \
           include/VertexData.h \
           include/DrawWidgetCF.h
           #include/CustomiseForm.h
           #include/CustomiseFormSingleton.h
           #include/ContextMenu.h

SOURCES += src/DrawWidget.cpp \
           src/FileExx.cpp \
           src/GraphData.cpp \
           src/main.cpp \
           src/MainWindow.cpp \
           src/ModelData.cpp \
           src/ShuffleIndexes.cpp \
           src/SimulationWorker.cpp \
           src/VertexData.cpp \
           #src/DrawWidgetMW.cpp 
           #src/CustomiseForm.cpp
           #src/CustomiseFormSingleton.cpp
           src/DrawWidgetCF.cpp
           #src/ContextMenu.cpp
