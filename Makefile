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
	mkdir -p ${DESTDIR}/var/openpanel/modules/Amavis.module
	mkdir -p ${DESTDIR}/var/openpanel/conf/staging/Amavis
	cp -rf ./amavismodule.app    ${DESTDIR}/var/openpanel/modules/Amavis.module/
	ln -sf amavismodule.app/exec ${DESTDIR}/var/openpanel/modules/Amavis.module/action
	cp     module.xml          ${DESTDIR}/var/openpanel/modules/Amavis.module/module.xml
	install -m 755 verify      ${DESTDIR}/var/openpanel/modules/Amavis.module/verify


clean:
	rm -f *.o *.exe
	rm -rf amavismodule.app
	rm -f amavismodule


SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -g $<
