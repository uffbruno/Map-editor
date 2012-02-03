#the compiler
CC=g++

#compiler flags, for extra error-and-warning intolerance
CPPFLAGS=-Wall -Wextra -pedantic -Weffc++ -Werror -g

#all these link flags is for linking Allegro 5 statically with my application
#so there is no need to send dlls along with executable and resources to distribution
A5LINKFLAGS=-lallegro-static -lallegro_font-static -lallegro_ttf-static -lallegro_image-static -lallegro_primitives-static
WIN32LINKFLAGS=-lopengl32 -lwinmm -luuid -lgdiplus -lfreetype -lole32 -lgdi32 -lkernel32 -lpsapi -lshlwapi -static-libgcc -static-libstdc++

#some shell commands
RM=rm
RMDIR=rmdir
MKDIR=mkdir
CP=cp

#directory where all source code is located
SRCDIR=src

#directory where all objects are located
#TODO: adapt this makefile to generate *.o -> build/*.o
OBJDIR=.

#directory where our final executable will be located
BINDIR=.

#directory for the distribution
DISTDIR=dist

#directory with the app's resources
RESOURCESDIR=resources

# 'make' looks for objects in the directories specified by VPATH
VPATH=$(SRCDIR);$(OBJDIR);$(BINDIR)
COMMON_OBJECTS=button.o animation.o character.o map2d.o sprite.o bounding_box.o char_2d.o mapgrid.o tile.o
MAPEDITOR_OBJECTS=main.o
TESTGUI_OBJECTS=testgui_main.o
EXECUTABLE=mapeditor.exe
TESTGUI=testgui

all: $(EXECUTABLE) $(TESTGUI)

$(EXECUTABLE): $(COMMON_OBJECTS) $(MAPEDITOR_OBJECTS)
	$(CC) -o $(BINDIR)\$(EXECUTABLE) $(COMMON_OBJECTS) $(MAPEDITOR_OBJECTS) $(A5LINKFLAGS) $(WIN32LINKFLAGS)

$(TESTGUI): $(COMMON_OBJECTS) $(TESTGUI_OBJECTS)
	$(CC) -o $(BINDIR)\$(TESTGUI).exe $(COMMON_OBJECTS) $(TESTGUI_OBJECTS) $(A5LINKFLAGS) $(WIN32LINKFLAGS)
    
#header file dependencies
main.o: button.hpp
button.o: button.hpp
mapgrid.o: mapgrid.hpp
animation.o: animation.hpp
character.o: character.hpp
map2d.o: map2d.hpp
sprite.o: sprite.hpp
bounding_box.o: bounding_box.hpp
char_2d.o: char_2d.hpp
tile.o: tile.hpp
testgui_main.o: button.hpp mapgrid.hpp

dist: $(EXECUTABLE)
	$(MKDIR) $(DISTDIR)
	$(CP) $(BINDIR)\$(EXECUTABLE) $(DISTDIR)
	$(CP) -r $(RESOURCESDIR) $(DISTDIR)
	$(CP) README $(DISTDIR)
	strip $(DISTDIR)\$(EXECUTABLE)
    
dist-zip: dist
	7z a $(DISTDIR).zip $(DISTDIR)\* -r
    
clean: clean-testgui clean-mapeditor
	$(RM) $(COMMON_OBJECTS)

.PHONY: clean-dist
clean-dist:
	$(RM) -rf $(DISTDIR)
	$(RM) $(DISTDIR).zip
    
.PHONY: clean-mapeditor
clean-mapeditor:
	$(RM) $(MAPEDITOR_OBJECTS) $(BINDIR)\$(EXECUTABLE)
    
.PHONY: clean-testgui
clean-testgui:
	$(RM) $(TESTGUI_OBJECTS) $(BINDIR)\$(TESTGUI).exe