/*
   +----------------------------------------------------------------------+
   | PHP Version 5 / dmtx                                                 |
   +----------------------------------------------------------------------+
   | Copyright (c) 2006-2007 Mikko Koppanen                               |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Mikko Kopppanen <mkoppanen@php.net>                          |
   +----------------------------------------------------------------------+
*/

#ifndef PHP_DMTX_H
/* Prevent double inclusion */
#define PHP_DMTX_H

/* Define Extension Properties */
#define PHP_DMTX_EXTNAME "dmtx"
#define PHP_DMTX_EXTVER  "0.0.2-dev"

#define PHP_DMTX_SC_NAME "dmtx"
#define PHP_DMTX_READ_SC_NAME "dmtxRead"
#define PHP_DMTX_WRITE_SC_NAME "dmtxWrite"
#define PHP_DMTX_EXCEPTION_SC_NAME "dmtxException"

#define DMTXWRITE_BUFFER_SIZE 4096

/* Import configure options when building 
	outside of the PHP source tree */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/* Include PHP Standard Header */
#include "php.h"

/* Include the dmtx header */
#include <dmtx.h>

#ifdef DMTX_IMAGEMAGICK_OLD
#  include <wand/magick-wand.h>
#else
#  include <wand/MagickWand.h>
#endif

/* Some extra headers */
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"
#include "ext/standard/php_string.h"
#include "ext/standard/info.h"
#include "php_ini.h"
#include "ext/standard/streamsfuncs.h"

/* Structure for dmtx object. */
typedef struct _php_dmtx_object  {
	zend_object zo;
} php_dmtx_object;

typedef struct _php_dmtx_scan_region {
	long x_min;
	long x_max;
	long y_min;
	long y_max;
	zend_bool active;
} php_dmtx_scan_region;

typedef struct _php_dmtx_read_opts {
	long timeout_ms;
	long start;
	long limit;
	long symbol;
	long shrink;
} php_dmtx_read_opts;

/* Structure for dmtx object. */
typedef struct _php_dmtx_read_object  {
	zend_object zo;
	MagickWand *magick_wand;
	php_dmtx_read_opts options;
	php_dmtx_scan_region scan_region;
} php_dmtx_read_object;

/* Structure for dmtx object. */
typedef struct _php_dmtx_write_object  {
	zend_object zo;
	char message[DMTXWRITE_BUFFER_SIZE];
	int message_len;
	MagickWand *magick_wand;
} php_dmtx_write_object;

/* Method declarations, dmtx class is just 
 * for holding the constants */

/* dmtxRead class */
PHP_METHOD(dmtxread, __construct);
PHP_METHOD(dmtxread, loadfile);
PHP_METHOD(dmtxread, loadstring);
PHP_METHOD(dmtxread, settimeout);
PHP_METHOD(dmtxread, setlimit);
PHP_METHOD(dmtxread, setshrink);
PHP_METHOD(dmtxread, setsymbolshape);
PHP_METHOD(dmtxread, setscanregion);

PHP_METHOD(dmtxread, gettimeout);
PHP_METHOD(dmtxread, getlimit);
PHP_METHOD(dmtxread, getshrink);
PHP_METHOD(dmtxread, getsymbolshape);
PHP_METHOD(dmtxread, getscanregion);

PHP_METHOD(dmtxread, getinfo);
PHP_METHOD(dmtxread, unsetscanregion);

/* dmtxWrite class */
PHP_METHOD(dmtxwrite, __construct);
PHP_METHOD(dmtxwrite, setmessage);
PHP_METHOD(dmtxwrite, save);

#define PHP_DMTX_NO_FILENAME_ERROR 1
#define PHP_DMTX_ALLOW_URL_FOPEN_ERROR 2
#define PHP_DMTX_UNABLE_TO_READ_FILENAME_ERROR 3
#define PHP_DMTX_FILENAME_TOO_LONG_ERROR 4
#define PHP_DMTX_OPEN_BASEDIR_ERROR 5
#define PHP_DMTX_SAFE_MODE_ERROR 6

/* Define the entry point symbol
 * Zend will use when loading this module */
extern zend_module_entry dmtx_module_entry;
#define phpext_dmtx_ptr &dmtx_module_entry

#endif /* PHP_DMTX_H */
