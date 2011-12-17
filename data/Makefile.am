
EXTRA_DIST =
CLEANFILES =

mimedir = $(prefix)/share/mime/packages
mime_DATA = raw-thumbnailer.xml

EXTRA_DIST += raw-thumbnailer.xml

# Thumbnailer
thumbnailerdir = $(datadir)/thumbnailers
thumbnailer_in_files = raw.thumbnailer.in
thumbnailer_DATA = $(thumbnailer_in_files:.thumbnailer.in=.thumbnailer)

raw.thumbnailer: $(thumbnailer_in_files) mime-type-list.txt thumbnailer.sh
	$(AM_V_GEN)sed -e "s|\@BINDIR\@|$(bindir)|" $< > $@ &&\
	$(SHELL) $(srcdir)/thumbnailer.sh $(srcdir)/mime-type-list.txt >> $@

EXTRA_DIST += \
	$(thumbnailer_in_files) \
	mime-type-list.txt \
	mime-functions.sh \
	thumbnailer.sh

CLEANFILES += \
	$(thumbnailer_DATA)
