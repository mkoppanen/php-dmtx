PHP_ARG_WITH(dmtx, whether to enable the dmtx extension,
[ --with-dmtx[=DIR]   Enables the dmtx extension.], no)

PHP_ARG_WITH(dmtx-imagemagick-dir, ImageMagick installation directory,
[ --with-dmtx-imagemagick-dir[=DIR]   ImageMagick installation directory.], no)

if test $PHP_DMTX != "no"; then

	AC_MSG_CHECKING(for dmtx.h header)
	
	for i in $PHP_DMTX /usr /usr/local /opt;
	do
		test -r $i/include/dmtx.h && DMTX_PATH=$i && break
	done
	
	if test -z "$DMTX_PATH"; then
		AC_MSG_ERROR(Cannot locate dmtx.h header)
	fi

	AC_MSG_RESULT(found in $DMTX_PATH/include/dmtx.h)

	AC_MSG_CHECKING(ImageMagick MagickWand API configuration program)
	
	for i in $PHP_DMTX_IMAGEMAGICK_DIR /usr/local /usr;
	do
		test -r $i/bin/MagickWand-config && WAND_BINARY=$i/bin/MagickWand-config && DMTX_OLD_IM=true && break
	done

	if test -z "$WAND_BINARY"; then
		AC_MSG_ERROR(not found. Please provide a path to MagickWand-config)
	else
		AC_MSG_RESULT(found in $WAND_BINARY)
	fi
	
	if test ! -z "$DMTX_OLD_IM"; then
		AC_DEFINE(DMTX_IMAGEMAGICK_OLD,1,[ ])
	fi
	
	IMAGEMAGICK_CFLAGS=`$WAND_BINARY --cflags`
	IMAGEMAGICK_LDFLAGS=`$WAND_BINARY --ldflags`
	IMAGEMAGICK_LIBS=`$WAND_BINARY --libs`
	
	PHP_EVAL_INCLINE("$IMAGEMAGICK_CFLAGS", DMTX_SHARED_LIBADD)
	PHP_EVAL_LIBLINE("$IMAGEMAGICK_LDFLAGS $IMAGEMAGICK_LIBS", DMTX_SHARED_LIBADD)

	AC_DEFINE(HAVE_DMTX_IMAGEMAGICK,1,[ ])

	AC_DEFINE(HAVE_DMTX,1,[ ])
	PHP_ADD_LIBRARY_WITH_PATH(dmtx, $DMTX_PATH/lib, DMTX_SHARED_LIBADD)
	PHP_ADD_INCLUDE($DMTX_PATH/include)
	
	PHP_SUBST(DMTX_SHARED_LIBADD)
	PHP_NEW_EXTENSION(dmtx, dmtx.c, $ext_shared)
fi
