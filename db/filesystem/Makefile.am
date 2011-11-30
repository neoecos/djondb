AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

all_includes = -I$(srcdir)/../util -I$(srcdir)/../bson

if DEBUG
   AM_CFLAGS = -g3 -O0 $(all_includes)
   AM_CPPFLAGS = -g3 -O0 $(all_includes)
else
   AM_CFLAGS = -O2 $(all_includes)
   AM_CPPFLAGS = -O2 $(all_includes)
endif

lib_LTLIBRARIES = lib@LIB_NAME@-@DJON_FILESYSTEM_API_VERSION@.la

lib@LIB_NAME@_@DJON_FILESYSTEM_API_VERSION@_la_SOURCES = bsoninputstream.cpp \
                                                         bsonoutputstream.cpp \
                                                         fileinputoutputstream.cpp \
                                                         fileinputstream.cpp \
                                                         fileoutputstream.cpp \
                                                         networkinputstream.cpp \
                                                         networkoutputstream.cpp \
                                                         bsoninputstream.h \
                                                         bsonoutputstream.h \
                                                         defs.h \
                                                         fileinputoutputstream.h \
                                                         fileinputstream.h \
                                                         fileoutputstream.h \
                                                         inputstream.h \
                                                         networkinputstream.h \
                                                         networkoutputstream.h \
                                                         outputstream.h

library_includedir=$(includedir)/@LIB_NAME@-1.0/@LIB_NAME@
#library_include_HEADERS = bson.h

@LIB_NAME@_libincludedir = $(libdir)/@LIB_NAME@-1.0/include
nodist_@LIB_NAME@_libinclude_HEADERS = config.h

