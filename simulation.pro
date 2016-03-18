######################################################################
# Automatically generated by qmake (2.01a) ?? ???. 20 18:42:55 2013
######################################################################

TEMPLATE = app
TARGET = bin/simulation
DEPENDPATH += . include src
INCLUDEPATH += . include
OBJECTS_DIR = tmp
MOC_DIR     = tmp
QT += widgets

# Input
HEADERS += include/FileExx.h \
           include/MainWindow.h \
           include/ModelData.h \
           include/ShuffleIndexes.h \
           include/SimulationWorker.h \
           include/Editor.h \
           include/DrawWidgetCF.h

SOURCES += src/FileExx.cpp \
           src/main.cpp \
           src/MainWindow.cpp \
           src/ModelData.cpp \
           src/ShuffleIndexes.cpp \
           src/SimulationWorker.cpp \
           src/Editor.cpp \
           src/DrawWidgetCF.cpp
