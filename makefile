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
COMMON_OBJECTS=button.o animation.o character.o map2d.o sprite.o bounding_box.o char_2d.o mapgrid.o tile.o map_editor.o
MAPEDITOR_OBJECTS=main.o
EXECUTABLE=mapeditor.exe

all: $(EXECUTABLE)

$(EXECUTABLE): $(COMMON_OBJECTS) $(MAPEDITOR_OBJECTS)
	$(CC) -o $(BINDIR)\$(EXECUTABLE) $(COMMON_OBJECTS) $(MAPEDITOR_OBJECTS) $(A5LINKFLAGS) $(WIN32LINKFLAGS)
    
#header file dependencies
main.o: map_editor.hpp
button.o: button.hpp
mapgrid.o: mapgrid.hpp map2d.hpp
animation.o: animation.hpp bounding_box.hpp
character.o: character.hpp game_object.hpp
map2d.o: map2d.hpp tile.hpp
sprite.o: sprite.hpp animation.hpp
bounding_box.o: bounding_box.hpp vector2d.hpp
char_2d.o: char_2d.hpp character.hpp
tile.o: tile.hpp game_object.hpp
map_editor.o: map_editor.hpp mapgrid.hpp button.hpp

dist: $(EXECUTABLE)
	$(MKDIR) $(DISTDIR)
	$(CP) $(BINDIR)\$(EXECUTABLE) $(DISTDIR)
	$(CP) -r $(RESOURCESDIR) $(DISTDIR)
	$(CP) README $(DISTDIR)
	strip $(DISTDIR)\$(EXECUTABLE)
    
dist-zip: dist
	7z a $(DISTDIR).zip $(DISTDIR)\* -r
    
clean:
	$(RM) $(COMMON_OBJECTS) $(MAPEDITOR_OBJECTS) $(EXECUTABLE)

.PHONY: clean-dist
clean-dist:
	$(RM) -rf $(DISTDIR)
	$(RM) $(DISTDIR).zip