if BUILD_EXAMPLE_MODULE

seedlibdir = ${libdir}/seed

seedlib_LTLIBRARIES = \
	libseed_example.la

libseed_example_la_SOURCES = \
	seed-example.c

AM_CPPFLAGS = \
	-I@top_srcdir@/libseed/ \
	$(GOBJECT_INTROSPECTION_CFLAGS) \
	$(SEED_DEBUG_CFLAGS) \
	$(SEED_PROFILE_CFLAGS)

libseed_example_la_LDFLAGS = \
	-module -avoid-version \
	$(GOBJECT_INTROSPECTION_LDFLAGS)

libseed_example_la_LIBADD = \
	$(top_builddir)/libseed/libseed.la \
	$(SEED_PROFILE_LIBS)

endif



