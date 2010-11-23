# This file is part of OpenPanel - The Open Source Control Panel
# OpenPanel is free software: you can redistribute it and/or modify it 
# under the terms of the GNU General Public License as published by the Free 
# Software Foundation, using version 3 of the License.
#
# Please note that use of the OpenPanel trademark may be subject to additional 
# restrictions. For more information, please visit the Legal Information 
# section of the OpenPanel website on http://www.openpanel.com/

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
	/usr/lib/openpanel-core/libcoremodule.a -lsqlite3

install:
	cp -rf ./amavismodule.app debian/openpanel-mod-amavis//var/openpanel/modules/Amavis.module/
	cp module.xml debian/openpanel-mod-amavis/var/openpanel/modules/Amavis.module/module.xml
	cp *.html debian/openpanel-mod-amavis/var/openpanel/modules/Amavis.module/
	install -m 664 etc/sqlite.in debian/openpanel-mod-amavis/etc/openpanel/amavis/sqlite.in
	install -m 755 verify debian/openpanel-mod-amavis/var/openpanel/modules/Amavis.module/verify
	install -m 755 fixconfig debian/openpanel-mod-amavis/var/openpanel/modules/Amavis.module/fixconfig
	cp -f ./etc/debian-amavis debian/openpanel-mod-amavis/etc/amavis/conf.d/


clean:
	rm -f *.o *.exe
	rm -rf amavismodule.app
	rm -f amavismodule


SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -g $<
