PHP_ARG_WITH(dmtx, whether to enable the dmtx extension,
[ --with-dmtx[=DIR]   Enables the dmtx extension.], no)

PHP_ARG_WITH(dmtx-imagemagick-dir, ImageMagick installation directory,
[ --with-dmtx-imagemagick-dir[=DIR]   ImageMagick installation directory.], no)

if test $PHP_DMTX != "no"; then

  AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  if test "x$PKG_CONFIG" = "xno"; then
    AC_MSG_RESULT([pkg-config not found])
    AC_MSG_ERROR([Please reinstall the pkg-config distribution])
  fi

  AC_MSG_CHECKING(for libdmtx)

  if test "x$PHP_DMTX" != "xyes"; then
    export PKG_CONFIG_PATH="${PHP_DMTX}/lib/pkgconfig"
  fi

  if $PKG_CONFIG --exists libdmtx; then
    PHP_DMTX_VERSION=`$PKG_CONFIG libdmtx --modversion`
    PHP_DMTX_PREFIX=`$PKG_CONFIG libdmtx --variable=prefix`

    AC_MSG_RESULT([found version $PHP_DMTX_VERSION, under $PHP_DMTX_PREFIX])
    PHP_DMTX_LIBS=`$PKG_CONFIG libdmtx --libs`
    PHP_DMTX_INCS=`$PKG_CONFIG libdmtx --cflags`
  else
    AC_MSG_ERROR(Unable to find libdmtx installation)
  fi

# ImageMagick has the config program:
# bin/Wand-config
# bin/MagickWand-config
#
# Sets IM_WAND_BINARY
#
  m4_include(imagemagick.m4)
  IM_FIND_IMAGEMAGICK(6002004)

  ORIG_LIBS="$LIBS"
  LIBS="$IM_IMAGEMAGICK_LIBS"

  AC_CHECK_FUNC([MagickExportImagePixels],
                [],
                [AC_MSG_ERROR([not found])])

  LIBS="$ORIG_LIBS"

  PHP_DMTX_LIBS="$PHP_DMTX_LIBS $IM_IMAGEMAGICK_LIBS"
  PHP_DMTX_INCS="$PHP_DMTX_INCS $IM_IMAGEMAGICK_CFLAGS"

  PHP_EVAL_LIBLINE($PHP_DMTX_LIBS, DMTX_SHARED_LIBADD)
  PHP_EVAL_INCLINE($PHP_DMTX_INCS)

  AC_DEFINE(HAVE_DMTX,1,[ ])

  PHP_ADD_LIBRARY_WITH_PATH(dmtx, $DMTX_PATH/$PHP_LIBDIR, DMTX_SHARED_LIBADD)
  PHP_ADD_INCLUDE($DMTX_PATH/include)

  PHP_SUBST(DMTX_SHARED_LIBADD)
  PHP_NEW_EXTENSION(dmtx, dmtx.c, $ext_shared,,$PHP_DMTX_INCS)
fi
