include makeinclude

OBJ	= main.o sqlite_db.o cfgamavis.o version.o

all: module.xml amavismodule.exe
	grace mkapp amavismodule 

module.xml: module.def
	mkmodulexml < module.def > module.xml

version.cpp:
	grace mkversion version.cpp

amavismodule.exe: $(OBJ)
	$(LD) $(LDFLAGS) -o amavismodule.exe $(OBJ) $(LIBS) \
	../opencore/api/c++/lib/libcoremodule.a -lz -lssl -lsqlite3

clean:
	rm -f *.o *.exe
	rm -rf amavismodule.app
	rm -f amavismodule

SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -I../opencore/api/c++/include -c -g $<
