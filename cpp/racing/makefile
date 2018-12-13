CC=g++
CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -Ilibs/include
LDFLAGS=
LDLIBS=
ifeq ($(OS),Windows_NT)
	LDLIBS+=-lws2_32 -lmswsock -lpdcurses -lgdi32 -lcomdlg32
	LDFLAGS+=-static-libgcc -static-libstdc++ -static -lpthread
else
	LDLIBS+=-lncurses -ltinfo -lgpm -pthread
endif
vpath %.o obj
vpath %.cpp src
vpath %.cpp libs/src/netlink

src_daemon=server.cpp cockroach.cpp obstacle.cpp utils.cpp daemon.cpp
src_match=cockroach.cpp obstacle.cpp track.cpp state.cpp world.cpp client.cpp utils.cpp match.cpp
src_view=cockroach.cpp obstacle.cpp state.cpp world.cpp utils.cpp view.cpp
src_test=cockroach.cpp obstacle.cpp state.cpp world.cpp utils.cpp test.cpp

obj_daemon=$(addprefix obj/,$(src_daemon:.cpp=.o))
obj_match=$(addprefix obj/,$(src_match:.cpp=.o))
obj_view=$(addprefix obj/,$(src_view:.cpp=.o))
obj_test=$(addprefix obj/,$(src_test:.cpp=.o))

src_netlink=core.cpp smart_buffer.cpp socket.cpp socket_group.cpp util.cpp
obj_netlink=$(addprefix obj/libs/netlink/,$(src_netlink:.cpp=.o))

all: daemon match view test

daemon: $(obj_daemon) $(obj_netlink)

match: $(obj_match) $(obj_netlink)

view: $(obj_view)

test: $(obj_test)

obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CXX) -c $(CXXFLAGS) $^ -o $@

obj/libs/netlink/%.o: libs/src/netlink/%.cpp
	@mkdir -p obj/libs/netlink
	$(CXX) -c $(CXXFLAGS) $^ -o $@

clean:
	@$(RM) -r obj
	@$(RM) daemon match view test
	@$(RM) run_data.dat