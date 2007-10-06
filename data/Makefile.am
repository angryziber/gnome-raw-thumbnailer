

update-gconf.sh: raw-thumbnailer.schemas.in

raw-thumbnailer.schemas.in: schemas.sh mime-types.txt mime-functions.sh
	$(srcdir)/schemas.sh $(srcdir)/mime-types.txt $(bindir)     \
	> $@                                                            \
	2> update-gconf.sh


generated_schema_in_files = raw-thumbnailer.schemas.in 
schema_in_files = $(generated_schema_in_files)
schemadir = $(GCONF_SCHEMA_FILE_DIR)
schema_DATA = $(schema_in_files:.schemas.in=.schemas)
@INTLTOOL_SCHEMAS_RULE@


mimedir = $(prefix)/share/mime/packages
mime_DATA = raw-thumbnailer.xml

install-data-local: $(schema_DATA)
if GCONF_SCHEMAS_INSTALL
        if test -z "$(DESTDIR)" ; then \
        for p in $^ ; do \
                GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $$p 2>&1 > /dev/null; \
        done \
        fi
endif

EXTRA_DIST = \
	mime-types.txt                  \
	schemas.sh                      \
	mime-functions.sh               \
	raw-thumbnailer.xml 

CLEANFILES = \
        $(generated_schema_in_files)    \
        $(schema_DATA)                  \
        raw-thumbnailer.schemas.in      \
        update-gconf.sh

