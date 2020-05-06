# Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir) \
			  -I$(top_srcdir)/src \
			  -I$(top_srcdir)/src/prefs \
			  $(GLIB_CFLAGS) $(GTK_CFLAGS) $(LIBWIDGETS_CFLAGS)

noinst_LTLIBRARIES = libwidgets.la

libwidgets_la_SOURCES = \
	widget_bank_details.c \
	widget_bank_details.h \
	widget_bank_details.ui \
	widget_currency_details.c \
	widget_currency_details.h \
	widget_currency_details.ui \
	widget_currency_link.c \
	widget_currency_link.h \
	widget_currency_link.ui \
	widget_currency_popup.c \
	widget_currency_popup.h \
	widget_currency_popup.ui \
	widget_import_asso.c \
	widget_import_asso.h \
	widget_import_asso.ui \
	widget_import_files.c \
	widget_import_files.h \
	widget_import_files.ui

libwidgets_la_LIBADD= $(LIBWIDGETS_LIBS)

CLEANFILES = \
	*~

MAINTAINERCLEANFILES = \
	Makefile.in