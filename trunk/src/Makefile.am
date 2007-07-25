bin_PROGRAMS = gimmage
gimmage_SOURCES = AppWindow.cpp AppWindow.h \
ThreadLoadThumbs.cpp ThreadLoadThumbs.h \
IconView.cpp IconView.h ImageListColumns.h \
PrintOperation.cpp PrintOperation.h \
PrintPreviewWidget.cpp PrintPreviewWidget.h \
PagePreview.cpp PagePreview.h \
Preview.cpp Preview.h \
FileManager.cpp FileManager.h \
ImageEventBox.cpp ImageEventBox.h \
SaveDialog.cpp SaveDialog.h \
SaveErrorDialog.cpp SaveErrorDialog.h \
main.cpp defines.h

AM_CXXFLAGS = @CXXFLAGS@ $(DEPS_CFLAGS) -lcurl -lmagic
LDADD = $(DEPS_LIBS)
