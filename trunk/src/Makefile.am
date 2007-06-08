bin_PROGRAMS = gimmage
gimmage_SOURCES = AppWindow.cpp AppWindow.h \
FileManager.cpp FileManager.h \
ImageEventBox.cpp ImageEventBox.h \
SaveDialog.cpp SaveDialog.h \
SaveErrorDialog.cpp SaveErrorDialog.h \
main.cpp defines.h

AM_CXXFLAGS = @CXXFLAGS@ $(DEPS_CFLAGS) -lcurl -lmagic
LDADD = $(DEPS_LIBS)