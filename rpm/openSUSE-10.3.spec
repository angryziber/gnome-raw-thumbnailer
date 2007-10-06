#
# spec file for package raw-thumbnailer
#
# Copyright (c) 2007 Novell Inc.
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#

# norootforbuild


Summary: Digital camera RAW thumbnailing for Nautilus
Name: raw-thumbnailer
Version: 0.99
Release: 1
License: GNU General Public License (GPL)
Group: System/GUI/GNOME
%define prefix   /usr
%define sysconfdir /etc%{prefix}
Source0: %name-%{version}.tar.gz
BuildRoot: /var/tmp/%{name}-buildroot
BuildRequires: libopenraw, gnome-vfs2-devel, libgsf-devel
PreReq:       shared-mime-info

%description
Thumbnailer for Nautilus

%prep
%setup -q


%build
CFLAGS="$RPM_OPT_FLAGS" \
    ./configure --prefix=%{prefix} \
    --libdir=%{prefix}/%_lib \
    --sysconfdir=%{sysconfdir} \
    --localstatedir=/var/%{prefix}
make

%install
rm -rf $RPM_BUILD_ROOT
export GCONF_DISABLE_MAKEFILE_SCHEMA_INSTALL=1
DESTDIR=$RPM_BUILD_ROOT make install
unset GCONF_DISABLE_MAKEFILE_SCHEMA_INSTALL

%post
usr/bin/update-mime-database %{prefix}/share/mime >/dev/null
export GCONF_CONFIG_SOURCE=`usr/bin/gconftool-2 \
                             --get-default-source`
usr/bin/gconftool-2 --makefile-install-rule \
    %{sysconfdir}/gconf/schemas/raw-thumbnailer.schemas >/dev/null

%postun
usr/bin/update-mime-database /usr/share/mime >/dev/null

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc README NEWS COPYING ChangeLog

%{prefix}/bin/raw-thumbnailer
%{sysconfdir}/gconf/schemas/raw-thumbnailer.schemas
%{prefix}/share/mime/packages/raw-thumbnailer.xml

%changelog
