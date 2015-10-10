
TEMPLATE	= app
CONFIG		+= qt warn_on embed_manifest_exe
QT		+= xml
TARGET		= Ignitor
RC_FILE		+= resources.rc
OBJECTS_DIR = obj
MOC_DIR = moc
UI_DIR = forms
VPATH += forms src

#win32:debug:CONFIG += console 


# Input
HEADERS += fractal.h mainWindow.h function.h myRandom.h optionDialog.h
SOURCES += main.cpp mainWindow.cpp fractal.cpp function.cpp myRandom.cpp optionDialog.cpp
RESOURCES += ignitor.qrc
FORMS += mainOpt.ui functionTab.ui
