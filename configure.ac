#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])

m4_define(dc_major_version, 0)
m4_define(dc_minor_version, 1)
m4_define(dc_micro_version, 3)
m4_define(dc_version, dc_major_version.dc_minor_version.dc_micro_version)

AC_INIT([Droplet Collection], [dc_version], [aabe.65535@gmail.com], [droplet-collection])
AM_INIT_AUTOMAKE([-Wno-portability foreign dist-zip dist-bzip2])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([src/config.h])

AC_DEFINE(DC_MAJOR_VERSION, dc_major_version, [Droplet Collection major version])
AC_SUBST(DC_MAJOR_VERSION, dc_major_version)
AC_DEFINE(DC_MINOR_VERSION, dc_minor_version, [Droplet Collection minor version])
AC_SUBST(DC_MINOR_VERSION, dc_minor_version)
AC_DEFINE(DC_MICRO_VERSION, dc_micro_version, [Droplet Collection micro version])
AC_SUBST(DC_MICRO_VERSION, dc_micro_version)

AC_SUBST(DC_DATADIR, "${datadir}/droplet-collection")

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL

# Checks for libraries.
PKG_CHECK_MODULES([gtkmm], [gtkmm-2.4 >= 2.12.2])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 data/Makefile])
AC_OUTPUT
