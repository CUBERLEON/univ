CC=g++
CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -Ilibs/include -DSFML_STATIC -DSFML
LDFLAGS=-static-libgcc -static-libstdc++ -static -lpthread
LDLIBS=-lsfml-graphics-s -lsfml-window-s -lsfml-network-s -lsfml-system-s -pthread
ifeq ($(OS),Windows_NT)
	LDFLAGS+=-Llibs/lib/windows
	LDLIBS+=-lfreetype -ljpeg -lwinmm -lgdi32 -lopengl32 -lws2_32
else
	LDFLAGS+=-Llibs/lib/linux
	LDLIBS+=-ljpeg -lX11 -lpthread -lxcb -lxcb-randr -lxcb-image -lGL -lX11-xcb -ludev
endif

vpath %.o obj
vpath %.cpp src

src_sfml=SFMLrenderer.cpp SFMLinput.cpp
src_sys=Utils.cpp Time.cpp Polygon.cpp
src_main=Main.cpp Map.cpp Tank.cpp Block.cpp World.cpp Bullet.cpp Renderer.cpp Input.cpp Core.cpp ServerCore.cpp ClientCore.cpp Player.cpp
# src_main+=BrickBlock.cpp WaterBlock.cpp

src_sfml:=$(addprefix src/sfml/, $(src_sfml))
src_sys:=$(addprefix src/sys/, $(src_sys))
src_main:=$(addprefix src/, $(src_main))
obj_sfml:=$(subst src,obj,$(src_sfml:.cpp=.o))
obj_sys:=$(subst src,obj,$(src_sys:.cpp=.o))
obj_main:=$(subst src,obj,$(src_main:.cpp=.o))

all: game

game: $(obj_main) $(obj_sys) $(obj_sfml) #depend
	$(CC) $(LDFLAGS) $(obj_main) $(obj_sys) $(obj_sfml) $(LDLIBS) -o tanks_multiplayer

#include depend1

obj/%.o: src/%.cpp
	@mkdir -p obj/sfml
	@mkdir -p obj/sys
	@mkdir -p obj
	$(CXX) -c $(CXXFLAGS) $^ -o $@

depend: depend1

depend1: $(src_sys) $(src_main)
	@rm -f depend1
	$(CC) $(CXXFLAGS) -MM $^ > depend1

clean:
	@$(RM) -r obj
	@$(RM) -f depend1
	@$(RM) tanks_multiplayer

