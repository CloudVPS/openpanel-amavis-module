# This file is part of OpenPanel - The Open Source Control Panel
# OpenPanel is free software: you can redistribute it and/or modify it 
# under the terms of the GNU General Public License as published by the Free 
# Software Foundation, using version 3 of the License.
#
# Please note that use of the OpenPanel trademark may be subject to additional 
# restrictions. For more information, please visit the Legal Information 
# section of the OpenPanel website on http://www.openpanel.com/
%define version 0.9.3

%define libpath /usr/lib
%ifarch x86_64
  %define libpath /usr/lib64
%endif

Summary: Openpanel module to manage amavis
Name: openpanel-mod-amavis
Version: %version
Release: 1
License: GPLv2
Group: Development
Source: http://packages.openpanel.com/archive/openpanel-mod-amavis-%{version}.tar.gz
Patch1: openpanel-mod-amavis-00-makefile
BuildRoot: /var/tmp/%{name}-buildroot
Requires: openpanel-core >= 0.8.3
Requires: openpanel-mod-postfixcourier >= 0.8.3
Requires: amavisd-new
Requires: spamassassin
Requires: clamav
Requires: sqlite
Requires: clamav-db
Requires: clamd
Requires: perl-DBD-SQLite

%description
Openpanel module to manage amavis
This module can add mail/virusscan capabilities to email domains.

%prep
%setup -q -n openpanel-mod-amavis-%version
%patch1 -p0 -b .buildroot

%build
BUILD_ROOT=$RPM_BUILD_ROOT
./configure
make

%install
BUILD_ROOT=$RPM_BUILD_ROOT
rm -rf ${BUILD_ROOT}
mkdir -p ${BUILD_ROOT}/var/opencore/modules/Amavis.module
cp -rf ./amavismodule.app ${BUILD_ROOT}/var/opencore/modules/Amavis.module/
cp module.xml ${BUILD_ROOT}/var/opencore/modules/Amavis.module/module.xml
cp *.html ${BUILD_ROOT}/var/opencore/modules/Amavis.module/
mkdir -p ${BUILD_ROOT}/etc/openpanel/amavis
install -m 664 etc/sqlite.in ${BUILD_ROOT}/etc/openpanel/amavis/sqlite.in
install -m 755 verify ${BUILD_ROOT}/var/opencore/modules/Amavis.module/verify
install -m 755 fixconfig ${BUILD_ROOT}/var/opencore/modules/Amavis.module/fixconfig
install -m 755 RedHat/postinstall ${BUILD_ROOT}/var/opencore/modules/Amavis.module/postinstall

%post
mkdir -p /var/opencore/conf/staging/Amavis
chmod 775 /etc/openpanel/amavis
chown opencore:authd /var/opencore/conf/staging/Amavis
chown -R amavis:opencore /etc/openpanel/amavis/
[ -e /etc/openpanel/amavis/amavis.db ] || sqlite3 /etc/openpanel/amavis/amavis.db < /etc/openpanel/amavis/sqlite.in
chmod 664 /etc/openpanel/amavis/amavis.db
chgrp opencore /etc/openpanel/amavis/amavis.db
mkdir -p /etc/postfix/openpanel/amavis
chgrp opencore /etc/postfix/openpanel/amavis
chmod g+w /etc/postfix/openpanel/amavis
/var/opencore/modules/Amavis.module/postinstall
service postfix reload >/dev/null 2>&1
chkconfig --level 2345 spamassassin on
service spamassassin restart >/dev/null 2>&1 || echo "spamassassin failure"
chkconfig --level 2345 spamd on
service clamd restart >/dev/null 2>&1 || echo "clamd failure"
chkconfig --level 2345 amavisd on
service amavisd restart >/dev/null 2>&1 || echo "amavis failure"
      ln -sf /var/opencore/modules/Amavis.module/amavismodule.app/exec /var/opencore/modules/Amavis.module/action

%files
%defattr(-,root,root)
/
