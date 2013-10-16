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
  AC_MSG_CHECKING(ImageMagick MagickWand API configuration program)

  if test "$PHP_IMAGICK" != "yes"; then
    for i in "$PHP_IMAGICK" /usr/local /usr /opt /opt/local;
    do
      if test -r "${i}/bin/MagickWand-config"; then
        IM_WAND_BINARY="${i}/bin/MagickWand-config"
        PHP_DMTX_IMAGEMAGICK_PATH=$i
        break
      fi

      if test -r "${i}/bin/Wand-config"; then
        IM_WAND_BINARY="${i}/bin/Wand-config"
        PHP_DMTX_IMAGEMAGICK_PATH=$i
        break
      fi
    done
  else
    for i in /usr/local /usr /opt /opt/local;
    do
      if test -r "${i}/bin/MagickWand-config"; then
        IM_WAND_BINARY="${i}/bin/MagickWand-config"
        PHP_DMTX_IMAGEMAGICK_PATH=$i
        break
      fi

      if test -r "${i}/bin/Wand-config"; then
        IM_WAND_BINARY="${i}/bin/Wand-config"
        PHP_DMTX_IMAGEMAGICK_PATH=$i
        break
      fi
    done
  fi

  if test "x" = "x$IM_WAND_BINARY"; then
    AC_MSG_ERROR(not found. Please provide a path to MagickWand-config or Wand-config program.)
  fi
  AC_MSG_RESULT([found in $IM_WAND_BINARY])

  WAND_DIR="`$IM_WAND_BINARY --prefix`"

  PHP_DMTX_IM_LIBS=`$IM_WAND_BINARY --libs`
  PHP_DMTX_IM_INCS=`$IM_WAND_BINARY --cflags`

  ORIG_LIBS="$LIBS"
  LIBS="$PHP_DMTX_IM_LIBS"

  AC_CHECK_FUNC([MagickExportImagePixels],
                [],
                [AC_MSG_ERROR([not found])])

  LIBS="$ORIG_LIBS"

  PHP_DMTX_LIBS="$PHP_DMTX_LIBS $PHP_DMTX_IM_LIBS"
  PHP_DMTX_INCS="$PHP_DMTX_INCS $PHP_DMTX_IM_INCS"

  PHP_EVAL_LIBLINE($PHP_DMTX_LIBS, DMTX_SHARED_LIBADD)
  PHP_EVAL_INCLINE($PHP_DMTX_INCS)

  AC_DEFINE(HAVE_DMTX,1,[ ])

  PHP_ADD_LIBRARY_WITH_PATH(dmtx, $DMTX_PATH/lib, DMTX_SHARED_LIBADD)
  PHP_ADD_INCLUDE($DMTX_PATH/include)

  PHP_SUBST(DMTX_SHARED_LIBADD)
  PHP_NEW_EXTENSION(dmtx, dmtx.c, $ext_shared,,$PHP_DMTX_INCS)
fi
