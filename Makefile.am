SUBDIRS = src data po

#appdir = $(datadir)/applications
#app_in_files = gnome-hello.desktop.in
#app_DATA = $(app_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

## We dist autogen.sh since this is an example program
## Real-world programs do not need to distribute autogen.sh

EXTRA_DIST = \
	ChangeLog               \
        xmldocs.make            \
        intltool-extract.in     \
        intltool-merge.in       \
        intltool-update.in

DISTCLEANFILES = intltool-extract intltool-merge intltool-update \
                 gnome-doc-utils.make

