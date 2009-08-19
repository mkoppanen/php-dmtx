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

#include "php_dmtx.h"

/* Handlers */
static zend_object_handlers php_dmtx_object_handlers;
static zend_object_handlers php_dmtx_read_object_handlers;
static zend_object_handlers php_dmtx_write_object_handlers;

/* Class entries */
zend_class_entry *php_dmtx_sc_entry;
zend_class_entry *php_dmtx_read_sc_entry;
zend_class_entry *php_dmtx_write_sc_entry;
zend_class_entry *php_dmtx_exception_class_entry;

#ifdef HAVE_DMTX_IMAGEMAGICK

#define PHP_DMTX_THROW_IMAGE_EXCEPTION(magick_wand, alternative_message) \
{ \
	ExceptionType severity; \
	char *message = MagickGetException(magick_wand, &severity); \
	MagickClearException(magick_wand); \
	if(message != NULL && strlen(message)) { \
		zend_throw_exception(php_dmtx_exception_class_entry, message, 1 TSRMLS_CC); \
		message = (char *)MagickRelinquishMemory(message); \
		message = (char *)NULL; \
		return; \
	} else { \
		zend_throw_exception(php_dmtx_exception_class_entry, alternative_message, 1 TSRMLS_CC); \
		return; \
	} \
} \

#endif

#define PHP_DMTX_THROW_GENERIC_EXCEPTION(message) \
	zend_throw_exception(php_dmtx_exception_class_entry, message, 1 TSRMLS_CC); \
	return; \

#define PHP_DMTX_REGISTER_CONST_LONG(const_name, value) \
	zend_declare_class_constant_long(php_dmtx_sc_entry, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC); \

#define PHP_DMTX_THROW_RW_ERROR_EXCEPTION(filename, code) \
	switch(code) { \
		case PHP_DMTX_NO_FILENAME_ERROR: \
			zend_throw_exception(php_dmtx_exception_class_entry, "No filename specified", 1 TSRMLS_CC); \
			return; \
		break; \
		case PHP_DMTX_ALLOW_URL_FOPEN_ERROR: \
 			zend_throw_exception(php_dmtx_exception_class_entry, "Tried to open from an url but allow_url_fopen is off", 1 TSRMLS_CC); \
			return; \
		break; \
		case PHP_DMTX_UNABLE_TO_READ_FILENAME_ERROR: \
			zend_throw_exception(php_dmtx_exception_class_entry, "Unknown error while reading the filename", 1 TSRMLS_CC); \
			return; \
		break; \
		case PHP_DMTX_FILENAME_TOO_LONG_ERROR: \
			zend_throw_exception(php_dmtx_exception_class_entry, "Filename length exceeds the maximum allowed size", 1 TSRMLS_CC); \
			return; \
		break; \
		case PHP_DMTX_SAFE_MODE_ERROR: \
			zend_throw_exception_ex(php_dmtx_exception_class_entry, 1 TSRMLS_CC, "Safe mode restriction in effect. User is not allowed to access file(%s)", filename); \
			return; \
		break; \
		case PHP_DMTX_OPEN_BASEDIR_ERROR: \
			zend_throw_exception_ex(php_dmtx_exception_class_entry, 1 TSRMLS_CC, "Open basedir restriction in effect. File(%s) is not within allowed path(s)", filename); \
			return; \
		break; \
	} \


static
	ZEND_BEGIN_ARG_INFO_EX(dmtxread_construct_args, 0, 0, 0)
		ZEND_ARG_INFO(0, filename)
	ZEND_END_ARG_INFO()

static
	ZEND_BEGIN_ARG_INFO_EX(dmtxread_loadfile_args, 0, 0, 1)
		ZEND_ARG_INFO(0, filename)
	ZEND_END_ARG_INFO()

static
	ZEND_BEGIN_ARG_INFO_EX(dmtxread_loadstring_args, 0, 0, 1)
		ZEND_ARG_INFO(0, image_string)
	ZEND_END_ARG_INFO()

static
	ZEND_BEGIN_ARG_INFO_EX(dmtxread_setscanregion_args, 0, 0, 4)
		ZEND_ARG_INFO(0, scan_region_x)
		ZEND_ARG_INFO(0, scan_region_y)
		ZEND_ARG_INFO(0, scan_region_width)
		ZEND_ARG_INFO(0, scan_region_height)
	ZEND_END_ARG_INFO()

static
	ZEND_BEGIN_ARG_INFO_EX(dmtxread_getinfo_args, 0, 0, 2)
		ZEND_ARG_INFO(0, scan_gap)
		ZEND_ARG_INFO(0, fix_errors)
	ZEND_END_ARG_INFO()

static function_entry php_dmtx_read_class_methods[] =
{
	PHP_ME(dmtxread, __construct, dmtxread_construct_args, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(dmtxread, loadfile, dmtxread_loadfile_args, ZEND_ACC_PUBLIC)
	PHP_ME(dmtxread, loadstring, dmtxread_loadstring_args, ZEND_ACC_PUBLIC)
	PHP_ME(dmtxread, setscanregion, dmtxread_setscanregion_args, ZEND_ACC_PUBLIC)
	PHP_ME(dmtxread, getinfo, dmtxread_getinfo_args, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

static
	ZEND_BEGIN_ARG_INFO_EX(dmtxwrite_construct_args, 0, 0, 0)
		ZEND_ARG_INFO(0, message)
	ZEND_END_ARG_INFO()

static
	ZEND_BEGIN_ARG_INFO_EX(dmtxwrite_setmessage_args, 0, 0, 1)
		ZEND_ARG_INFO(0, message)
	ZEND_END_ARG_INFO()

static
	ZEND_BEGIN_ARG_INFO_EX(dmtxwrite_save_args, 0, 0, 2)
		ZEND_ARG_INFO(0, filename)
		ZEND_ARG_INFO(0, symbol)
		ZEND_ARG_INFO(0, format)
	ZEND_END_ARG_INFO()

static function_entry php_dmtx_write_class_methods[] =
{
	PHP_ME(dmtxwrite, __construct, dmtxwrite_construct_args, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(dmtxwrite, setmessage, dmtxwrite_setmessage_args, ZEND_ACC_PUBLIC)
	PHP_ME(dmtxwrite, save, dmtxwrite_save_args, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

static function_entry php_dmtx_class_methods[] =
{
	{ NULL, NULL, NULL }
};

/* Count occurances */
int count_occurences_of(char needle, char *haystack TSRMLS_DC)
{
	int occurances = 0;

	if (haystack == (char *)NULL) {
		return 0;
	}

	while (*haystack != '\0') {

		if (*(haystack++) == needle) {
			occurances++;
		}
	}
	return occurances;
}

#ifdef HAVE_DMTX_IMAGEMAGICK
/* Get the filename */
char *get_pseudo_filename( char *pseudo_string TSRMLS_DC )
{
	char *filename = NULL;
	char *ptr = strchr(pseudo_string, ':');
	
	/* No colon */
	if(ptr == NULL) {
		return NULL;
	} else {
		++ptr; /* Move one position, removing colon from filename */
		filename = estrdup(ptr);
	}
	return filename;
}

/* Check if file is safe to read */
int check_file_safety(char *filename TSRMLS_DC)
{
	int pseudo_format = 0, needs_open_basedir_check = 1, i;
	char *absolute = NULL, *pseudo_filename;
#ifdef PHP_WIN32
	int formats = 20;
	char *no_basedir_check[] = {    "caption:", "clipboard:", "fractal:", "gradient:",
									"histogram:", "label:", "map:", "matte:",
									"null:", "plasma:", "preview:", "print:",
									"scan:", "stegano:", "unique:", "win:",
								    "xc:", "magick:", "pattern:", "http:", "ftp:" };
#else
	int formats = 21;
	char *no_basedir_check[] = {    "caption:", "clipboard:", "fractal:", "gradient:",
									"histogram:", "label:", "map:", "matte:",
									"null:", "plasma:", "preview:", "print:",
									"scan:", "stegano:", "unique:", "win:",
									"x:", "xc:", "magick:", "pattern:", "http:", "ftp:" };
#endif

	if (filename == (char *)NULL) {
		return PHP_DMTX_NO_FILENAME_ERROR;
	}

#ifdef PHP_WIN32
	if (count_occurences_of( ':', filename TSRMLS_CC) >= 2) {
#else
	if (count_occurences_of( ':', filename TSRMLS_CC) >= 1) {
#endif
		pseudo_format = 1;
	}

	if(pseudo_format == 1) {

		if (!PG( allow_url_fopen)) {
			if ((strncasecmp(filename, "http:", 5) == 0) || (strncasecmp(filename, "ftp:", 4) == 0)) {
				return PHP_DMTX_ALLOW_URL_FOPEN_ERROR;
			}
		}
	
		for (i = 0; i < formats; i++) {
			/* No open_basedir check needed */
			if (strncasecmp(filename, no_basedir_check[i], strlen(no_basedir_check[i])) == 0) {
				needs_open_basedir_check = 0;
				break;
			}
		}
	
		/* These formats potentially read images */
		if ( needs_open_basedir_check == 1 ) {
			pseudo_filename = get_pseudo_filename(filename TSRMLS_CC );
	
			if (pseudo_filename == NULL) {
				return PHP_DMTX_UNABLE_TO_READ_FILENAME_ERROR;
			}
	
			if (strlen(pseudo_filename) >= MAXPATHLEN) {
				efree(pseudo_filename);
				return PHP_DMTX_FILENAME_TOO_LONG_ERROR;
			}
	
			absolute = expand_filepath(pseudo_filename, NULL TSRMLS_CC);
	
			if(absolute == NULL) {
				efree(pseudo_filename);
				return PHP_DMTX_UNABLE_TO_READ_FILENAME_ERROR;
			}
	
			if (PG(safe_mode) && (!php_checkuid_ex(absolute, NULL, CHECKUID_CHECK_FILE_AND_DIR, CHECKUID_NO_ERRORS))) {
				efree(pseudo_filename);
				efree(absolute);
				return PHP_DMTX_SAFE_MODE_ERROR;
			}
			if (php_check_open_basedir_ex(absolute, 0 TSRMLS_CC)) {
				efree(pseudo_filename);
				efree(absolute);
				return PHP_DMTX_OPEN_BASEDIR_ERROR;
			}
	
			efree(pseudo_filename);
			efree(absolute);
		}
	} else {

		absolute = expand_filepath(filename, NULL TSRMLS_CC);

		if(absolute == NULL) {
			return PHP_DMTX_UNABLE_TO_READ_FILENAME_ERROR;
		}

		if (PG(safe_mode) && (!php_checkuid_ex(absolute, NULL, CHECKUID_CHECK_FILE_AND_DIR, CHECKUID_NO_ERRORS))) {
			efree(absolute);
			return PHP_DMTX_SAFE_MODE_ERROR;
		}
		if (php_check_open_basedir_ex(absolute, 0 TSRMLS_CC)) {
			efree(absolute);
			return PHP_DMTX_OPEN_BASEDIR_ERROR;
		}
		efree(absolute);
	}
	return 0; /* No error */
}
#endif

/* Extract dmtxImage */
char *php_dmtx_create_pnm_image(DmtxEncode *encode, int *image_size TSRMLS_DC)
{
	char header[32];
	int i, row, col, buf_size;
	char *buffer;
	
	memset(header, '\0', 32);
	sprintf(header, "P6 %d %d 255 ", encode->image->width, encode->image->height);

	if((encode->image->width * encode->image->height) >= INT_MAX) {
		return NULL;
	}

	buf_size = (sizeof(char) * ((encode->image->width * encode->image->height) * 3)) + strlen(header);
	buffer = (char *)emalloc(buf_size);

	if(!buffer) {
		return NULL;
	}

	strncpy(buffer, header, strlen(header));
	i = strlen(header);
	for(row = 0; row < encode->image->height; row++) {
		for(col = 0; col < encode->image->width; col++) {
			buffer[i++] = encode->image->pxl[(encode->image->height - row - 1) * encode->image->width + col].R;
			buffer[i++] = encode->image->pxl[(encode->image->height - row - 1) * encode->image->width + col].G;
			buffer[i++] = encode->image->pxl[(encode->image->height - row - 1) * encode->image->width + col].B;
		}
	}
	*image_size = buf_size;
	return buffer;
}
#ifdef HAVE_DMTX_IMAGEMAGICK
/* Create DmtxImage from MagickWand */
DmtxImage *php_create_dmtx_image_from_wand(MagickWand *magick_wand TSRMLS_DC)
{
	DmtxImage *image = NULL;
	PixelIterator *iterator;
	PixelWand **pixels;
	MagickPixelPacket pixel;
	long y, x, image_width, image_height;
	unsigned long width;
	int offset;
	double red, green, blue;

	MagickSetImageColorspace(magick_wand, RGBColorspace);

	iterator = NewPixelIterator(magick_wand);
	PixelResetIterator(iterator);

	image_width = MagickGetImageWidth(magick_wand);
	image_height = MagickGetImageHeight(magick_wand);

	image = dmtxImageMalloc(image_width, image_height);

	for(y = 0; y < (long) image_height; y++) {

		pixels = PixelGetNextIteratorRow(iterator,&width);

		if ((pixels == (PixelWand **) NULL)) {
			return NULL;
		}

		for(x = 0; x < (long) width; x++) {
			offset = ((image_height - y - 1) * width) + x;

			red = PixelGetRed( pixels[x] ) * 255;
			green = PixelGetGreen( pixels[x] ) * 255;
			blue = PixelGetBlue( pixels[x] ) * 255;

			image->pxl[offset].R = (unsigned char)(red > 0.0 ? red + 0.5 : red - 0.5);
			image->pxl[offset].G = (unsigned char)(green > 0.0 ? green + 0.5 : green - 0.5);
			image->pxl[offset].B = (unsigned char)(blue > 0.0 ? blue + 0.5 : blue - 0.5);
		}
	}
	iterator = DestroyPixelIterator(iterator);
	return image;
}
#else
/* If ImageMagick is not enabled, use this to write an image */
int php_dmtx_write_pnm_image(char *filename, char *image_string, int image_string_len TSRMLS_DC)
{
	php_stream *stream;
	stream = php_stream_open_wrapper(filename, "wb", ENFORCE_SAFE_MODE | REPORT_ERRORS, NULL);

	if(!stream) {
		return 0;
	}

	if(!php_stream_write(stream, image_string, image_string_len)) {
		return 0;
	}

	php_stream_close(stream);
	return 1;
}

/* Validate that pnm header is "about correct" */
int php_dmtx_validate_pnm_image(char *image_string, int *image_width, int *image_height TSRMLS_DC)
{
	int colors;
	int width, height;
	char s_width[16], s_height[16];

	/* Scan the header */
	sscanf(image_string, "P6 %s %s 255 ", s_width, s_height);
	
	/* Got header */
	if((strlen(s_width) == 0) || (strlen(s_height) == 0)) {
		return 0;
	}

	/* Scan the header again */
	sscanf(image_string, "P6 %d %d %d ", &width, &height, &colors);
	
	if(colors != 255) {
		return 0;
	}
	
	*image_width = width;
	*image_height = height;

	return 1;
}

DmtxImage *php_create_dmtx_image_from_pnm_string(char *image_string, int image_string_len TSRMLS_DC)
{
	DmtxImage *image = NULL;
	int width, height, header_len, first = 1, colors, i;
	char s_width[16], s_height[16];
	int x, y, offset;

	memset(s_width, '\0', 16);
	memset(s_height, '\0', 16);

	/* Scan the header */
	sscanf(image_string, "P6 %s %s 255 ", s_width, s_height);
	
	/* Got header */
	if(strlen(s_width) && strlen(s_height)) {
		header_len = 9 + strlen(s_width) + strlen(s_height);
	} else {
		return 0;
	}

	/* Scan the header again */
	sscanf(image_string, "P6 %d %d %d ", &width, &height, &colors);

	if(colors != 255) {
		return 0;
	}

	if((width * height) >= INT_MAX) {
		return 0;
	}

	image = dmtxImageMalloc(width, height);
	i = header_len;

	for(y = 0; y < (long) height; y++) {
		for(x = 0; x < (long) width; x++ ) {
			offset = (height - y - 1) * width + x;
			if(first == 1) {
				offset += header_len;
				first = 0;
			}
			image->pxl[offset].R = image_string[i++];
			image->pxl[offset].G = image_string[i++];
			image->pxl[offset].B = image_string[i++];
			if (i >= image_string_len) {
				break;
			}
		}
	}
	return image;
}
#endif

/* {{{ proto void dmtxRead::__construct([string filename])
	Constructs a new dmtxRead object */
PHP_METHOD(dmtxread, __construct)
{
	php_dmtx_read_object *intern;
	char *filename = NULL;
	int filename_len = 0;
	int image_width, image_height;

	if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "|s!", &filename, &filename_len) == FAILURE) {
		return;
	}

	intern = (php_dmtx_read_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if(filename != (char *)NULL && filename_len > 0) {
#ifdef HAVE_DMTX_IMAGEMAGICK
		MagickBooleanType status;
		int safety = 0;

		if ((safety = check_file_safety(filename)) != 0) {
			PHP_DMTX_THROW_RW_ERROR_EXCEPTION(filename, safety);
		}

		status = MagickReadImage(intern->magick_wand, filename);

		if(status == MagickFalse) {
			PHP_DMTX_THROW_IMAGE_EXCEPTION(intern->magick_wand, "Unable to read the image");
		}

		image_width = MagickGetImageWidth(intern->magick_wand);
		image_height = MagickGetImageHeight(intern->magick_wand);
#else
		int stream_len;
		php_stream *stream;
		long maxlen = PHP_STREAM_COPY_ALL;

		stream = php_stream_open_wrapper(filename, "rb", ENFORCE_SAFE_MODE | REPORT_ERRORS, NULL);
		
		if(!stream) {
			PHP_DMTX_THROW_GENERIC_EXCEPTION("Unable to read the image");
		}
	
		if ((stream_len = php_stream_copy_to_mem(stream, &(intern->image_string), maxlen, 0)) > 0) {
			php_stream_close(stream);
		} else {
			php_stream_close(stream);
			PHP_DMTX_THROW_GENERIC_EXCEPTION("Unable to read the image");
		}	

		if(!php_dmtx_validate_pnm_image(intern->image_string, &image_width, &image_height)) {
			PHP_DMTX_THROW_GENERIC_EXCEPTION("Unable to validate the image format");
		}
#endif
		intern->image_width = image_width;
		intern->image_height = image_height;
		intern->use_scan_region = 0;
	}
	return;
}
/* }}} */

/* {{{ proto bool dmtxRead::loadFile(string filename)
	Loads a file into the object */
PHP_METHOD(dmtxread, loadfile)
{
	php_dmtx_read_object *intern;
	char *filename;
	int filename_len;
	int image_width, image_height;

	if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &filename, &filename_len) == FAILURE) {
		return;
	}

	intern = (php_dmtx_read_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if(filename_len > 0) {
#ifdef HAVE_DMTX_IMAGEMAGICK
		MagickBooleanType status;
		int safety = 0;

		if ((safety = check_file_safety(filename)) != 0) {
			PHP_DMTX_THROW_RW_ERROR_EXCEPTION(filename, safety);
		}

		status = MagickReadImage(intern->magick_wand, filename);
		if(status == MagickFalse) {
			PHP_DMTX_THROW_IMAGE_EXCEPTION(intern->magick_wand, "Unable to read the image");
		}
#else
		int stream_len;
		php_stream *stream;
		long maxlen = PHP_STREAM_COPY_ALL;

		stream = php_stream_open_wrapper(filename, "rb", ENFORCE_SAFE_MODE | REPORT_ERRORS, NULL);
		
		if(!stream) {
			PHP_DMTX_THROW_GENERIC_EXCEPTION("Unable to read the image");
		}
	
		if ((stream_len = php_stream_copy_to_mem(stream, &(intern->image_string), maxlen, 0)) > 0) {
			php_stream_close(stream);
		} else {
			php_stream_close(stream);
			PHP_DMTX_THROW_GENERIC_EXCEPTION("Unable to read the image");
		}

		if(!php_dmtx_validate_pnm_image(intern->image_string, &image_width, &image_height)) {
			PHP_DMTX_THROW_GENERIC_EXCEPTION("Unable to validate the image format");
		}
#endif
		intern->image_width = image_width;
		intern->image_height = image_height;
		intern->use_scan_region = 0;
		RETURN_TRUE;
	} else {
		PHP_DMTX_THROW_GENERIC_EXCEPTION("Unable to read the image");
	}
}
/* }}} */

/* {{{ proto bool dmtxRead::loadString(string filename)
	Loads a string into the object */
PHP_METHOD(dmtxread, loadstring)
{
	php_dmtx_read_object *intern;
	char *image_string;
	int image_string_len;
	int image_width, image_height;

	if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &image_string, &image_string_len) == FAILURE) {
		return;
	}

	intern = (php_dmtx_read_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if(image_string_len > 0) {
#ifdef HAVE_DMTX_IMAGEMAGICK
		MagickBooleanType status;
		status = MagickReadImageBlob(intern->magick_wand, image_string, image_string_len);

		if(status == MagickFalse) {
			PHP_DMTX_THROW_IMAGE_EXCEPTION(intern->magick_wand, "Unable to read the image");
		}

		image_width = MagickGetImageWidth(intern->magick_wand);
		image_height = MagickGetImageHeight(intern->magick_wand);
#else
		intern->image_string = (char *)emalloc(image_string_len);
		memcpy(intern->image_string, image_string, image_string_len);

		if(!php_dmtx_validate_pnm_image(intern->image_string, &image_width, &image_height)) {
			PHP_DMTX_THROW_GENERIC_EXCEPTION("Unable to validate the image format");
		}
#endif
		intern->image_width = image_width;
		intern->image_height = image_height;
		intern->use_scan_region = 0;
		RETURN_TRUE;
	} else {
		PHP_DMTX_THROW_GENERIC_EXCEPTION("Unable to read the image");
	}
}
/* }}} */

/* {{{ proto bool dmtxRead::setScanRegion(int x, int y, int width, int height)
	Loads a file into the object */
PHP_METHOD(dmtxread, setscanregion)
{
	php_dmtx_read_object *intern;
	long scan_x, scan_y, scan_width, scan_height;

	if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "llll", &scan_x, &scan_y, &scan_width, &scan_height) == FAILURE) {
		return;
	}
	
	intern = (php_dmtx_read_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
#ifdef HAVE_DMTX_IMAGEMAGICK
	if(MagickGetNumberImages(intern->magick_wand) == 0) {
		PHP_DMTX_THROW_GENERIC_EXCEPTION("The object does not contain an image");
#else
	if(intern->image_string_len > 0 || intern->image_string == (char *)NULL) {
		PHP_DMTX_THROW_GENERIC_EXCEPTION("The object does not contain an image");	
#endif
	} else {
		if(((scan_x + scan_width) > (intern->image_width - 1)) ||
			((scan_y + scan_height) > (intern->image_height - 1))) {
			PHP_DMTX_THROW_GENERIC_EXCEPTION("The scan region is larger than the actual image size");
		}

		if((scan_x < 0) || (scan_y < 0)) {
			PHP_DMTX_THROW_GENERIC_EXCEPTION("The scan region coordinates can not be negative");
		}

		intern->scan_region_x = scan_x;
		intern->scan_region_y = scan_y;
		intern->scan_region_width = scan_width;
		intern->scan_region_height = scan_height;

		intern->use_scan_region = 1;
		RETURN_TRUE;
	}
}
/* }}} */

/* {{{ proto array dmtxRead::getInfo(int scan_gap, bool fix_errors)
	Fetches the information from the image */
PHP_METHOD(dmtxread, getinfo)
{
	php_dmtx_read_object *intern;
	DmtxDecode decode;
	DmtxPixelLoc p0, p1;
	DmtxRegion region;
	DmtxMessage *message;
	DmtxImage *image;
	long scan_gap = 0;
	int data_word_len, rotate_int;
	double rotate;
	zval *tmp_val;
	char buffer[32];
	zend_bool fix_errors;

	if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "lb", &scan_gap, &fix_errors) == FAILURE) {
		return;
	}	

	intern = (php_dmtx_read_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
#ifdef HAVE_DMTX_IMAGEMAGICK
	image = php_create_dmtx_image_from_wand(intern->magick_wand TSRMLS_CC);

	if(!image) {
		PHP_DMTX_THROW_IMAGE_EXCEPTION(intern->magick_wand, "Unable to read the image");
	}
#else
	image = php_create_dmtx_image_from_pnm_string(intern->image_string, intern->image_string_len TSRMLS_CC);

	if(!image) {
		PHP_DMTX_THROW_GENERIC_EXCEPTION("Unable to read the image");
	}
#endif

	if(scan_gap <= 0) {
		PHP_DMTX_THROW_GENERIC_EXCEPTION("The scan gap needs to be larger than zero");
	}

	if(intern->use_scan_region == 0) {
		p0.X = p0.Y = 0;
		p1.X = image->width - 1;
		p1.Y = image->height - 1;
	} else {
		p0.X = intern->scan_region_x;
		p1.X = intern->scan_region_x + intern->scan_region_width;

		p0.Y = intern->scan_region_y;
		p1.Y = intern->scan_region_y + intern->scan_region_height;
	}

	decode = dmtxDecodeStructInit(image, p0, p1, scan_gap);
	region = dmtxDecodeFindNextRegion(&decode);

	if(region.found == DMTX_REGION_EOF) {
		dmtxImageFree(&image);
		dmtxDecodeStructDeInit(&decode);
		RETURN_NULL();
	}

	message = dmtxDecodeMatrixRegion(&decode, &region, fix_errors);

	if(message != NULL) {

		/* Assemble an array of the values */
		data_word_len = dmtxGetSymbolAttribute(DmtxSymAttribDataWordLength, region.sizeIdx);
		rotate = (2 * M_PI) + (atan2(region.fit2raw[0][1], region.fit2raw[1][1]) - atan2(region.fit2raw[1][0], region.fit2raw[0][0])) / 2.0;
		rotate_int = (int)(rotate * 180/M_PI + 0.5);
      
		if(rotate_int >= 360) {
			rotate_int -= 360;
		}
		
		array_init(return_value);

		add_assoc_string(return_value, "message", (char *)message->output, 1);

		add_assoc_long(return_value, "matrix_width", dmtxGetSymbolAttribute(DmtxSymAttribSymbolRows, region.sizeIdx) );
		add_assoc_long(return_value, "matrix_height", dmtxGetSymbolAttribute(DmtxSymAttribSymbolCols, region.sizeIdx) );

		add_assoc_long(return_value, "codewords", message->outputIdx );
		add_assoc_long(return_value, "error_codewords", dmtxGetSymbolAttribute(DmtxSymAttribErrorWordLength, region.sizeIdx));

		add_assoc_long(return_value, "data_regions_horizontal", dmtxGetSymbolAttribute(DmtxSymAttribHorizDataRegions, region.sizeIdx));
		add_assoc_long(return_value, "data_regions_vertical", dmtxGetSymbolAttribute(DmtxSymAttribVertDataRegions, region.sizeIdx));

		add_assoc_long(return_value, "interleaved_blocks", dmtxGetSymbolAttribute(DmtxSymAttribInterleavedBlocks, region.sizeIdx));
		
		add_assoc_long(return_value, "rotation_angle", rotate_int);

		MAKE_STD_ZVAL(tmp_val);
		array_init(tmp_val);

		memset(buffer, '\0', 32);
		sprintf(buffer, "(%0.1f, %0.1f)", region.corners.c00.X, region.corners.c00.Y);
		add_assoc_string(tmp_val, "0", buffer, 1);

		memset(buffer, '\0', 32);
		sprintf(buffer, "(%0.1f, %0.1f)", region.corners.c10.X, region.corners.c10.Y);
		add_assoc_string(tmp_val, "1", buffer, 1);

		memset(buffer, '\0', 32);
		sprintf(buffer, "(%0.1f, %0.1f)", region.corners.c11.X, region.corners.c11.Y);
		add_assoc_string(tmp_val, "2", buffer, 1);

		memset(buffer, '\0', 32);
		sprintf(buffer, "(%0.1f, %0.1f)", region.corners.c01.X, region.corners.c01.Y);
		add_assoc_string(tmp_val, "3", buffer, 1);

		add_assoc_zval(return_value, "corners", tmp_val);

		dmtxDecodeStructDeInit(&decode);
		dmtxMessageFree(&message);
		dmtxImageFree(&image);
		return;
	} else {
		dmtxImageFree(&image);
		dmtxDecodeStructDeInit(&decode);
		RETURN_NULL();
	}
}
/* }}} */

/* {{{ proto void dmtxWrite::__construct([string message])
	Constructs the dmtxWrite object */
PHP_METHOD(dmtxwrite, __construct)
{
	php_dmtx_write_object *intern;
	char *message = NULL;
	int message_len = 0;

	if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "|s!", &message, &message_len) == FAILURE) {
		return;
	}
	
	if(message == (char *)NULL || message_len == 0) {
		return;
	}

	if(message_len >= DMTXWRITE_BUFFER_SIZE) {
		PHP_DMTX_THROW_GENERIC_EXCEPTION("The message is larger than the maximum allowed size");
	}

	intern = (php_dmtx_write_object *)zend_object_store_get_object(getThis() TSRMLS_CC);	
	memset(intern->message, '\0', DMTXWRITE_BUFFER_SIZE);
	strncpy(intern->message, message, message_len);
	intern->message_len = message_len;
	return;
}
/* }}} */

/* {{{ proto bool dmtxWrite::setMessage([string message])
	Sets the message on the object */
PHP_METHOD(dmtxwrite, setmessage)
{
 	php_dmtx_write_object *intern;
	char *message;
	int message_len;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &message, &message_len) == FAILURE) {
		return;
	}

	if(message_len >= DMTXWRITE_BUFFER_SIZE) {
		PHP_DMTX_THROW_GENERIC_EXCEPTION("The message is larger than the maximum allowed size");
	}

	intern = (php_dmtx_write_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

#if HAVE_DMTX_IMAGEMAGICK
	if(MagickGetNumberImages(intern->magick_wand) > 0) {
		ClearMagickWand(intern->magick_wand);
	}
#endif

	memset(intern->message, '\0', DMTXWRITE_BUFFER_SIZE);
	strncpy(intern->message, message, message_len);
	intern->message_len = message_len;

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool dmtxWrite::save(string filename, int mode[, string format] )
	Saves the message into a file */
PHP_METHOD(dmtxwrite, save)
{
 	php_dmtx_write_object *intern;
	char *filename, *format = NULL;
	int filename_len, format_len = 0, image_size;
	char *image_string;
	long symbol;
	DmtxEncode encode;
		
	if(zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "sl|s!", &filename, &filename_len, &symbol, &format, &format_len) == FAILURE) {
		return;
	}

	intern = (php_dmtx_write_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if(intern->message_len == 0) {
		PHP_DMTX_THROW_GENERIC_EXCEPTION("The object does not contain a message");
	}

	if((strlen(filename) + format_len + 1) >= MAXPATHLEN) {
		PHP_DMTX_THROW_GENERIC_EXCEPTION("The filename exceeds the maximum allowed length");
	}

	encode = dmtxEncodeStructInit();
	dmtxEncodeDataMatrix(&encode, intern->message_len, intern->message, symbol);
	image_string = php_dmtx_create_pnm_image(&encode, &image_size TSRMLS_CC);
	
	if(image_string == NULL) {
		dmtxEncodeStructDeInit(&encode);
		PHP_DMTX_THROW_GENERIC_EXCEPTION("Internal error");
	} else {
#ifdef HAVE_DMTX_IMAGEMAGICK
		char filename_with_format[MAXPATHLEN];
		int safety = 0;

		if ((safety = check_file_safety(filename)) != 0) {
			PHP_DMTX_THROW_RW_ERROR_EXCEPTION(filename, safety);
		}

		memset(filename_with_format, '\0', MAXPATHLEN);
		if(format != NULL && format_len > 0) {
			strncat(filename_with_format, format, format_len);
			strncat(filename_with_format, ":", 1);
		}
		strncat(filename_with_format, filename, filename_len);
	
		if(!MagickReadImageBlob(intern->magick_wand, image_string, image_size)) {
			dmtxEncodeStructDeInit(&encode);
			PHP_DMTX_THROW_IMAGE_EXCEPTION(intern->magick_wand, "Unable to write the image");
		}
	
		if(!MagickWriteImage(intern->magick_wand, filename_with_format)) {
			dmtxEncodeStructDeInit(&encode);
			PHP_DMTX_THROW_IMAGE_EXCEPTION(intern->magick_wand, "Unable to write the image");
		}
#else
		if(!php_dmtx_write_pnm_image(filename, image_string, image_size TSRMLS_CC)) {
			dmtxEncodeStructDeInit(&encode);
			PHP_DMTX_THROW_GENERIC_EXCEPTION("Unable to write the image");
		}
#endif
	}
	dmtxEncodeStructDeInit(&encode);
	efree(image_string);
	RETURN_TRUE;
}

/* Initialization */
static void php_dmtx_object_free_storage(void *object TSRMLS_DC)
{
	php_dmtx_object *intern = (php_dmtx_object *)object;
	
	if(!intern) {
		return;
	}

	zend_object_std_dtor(&intern->zo TSRMLS_CC);
	efree(intern);
}

static void php_dmtx_read_object_free_storage(void *object TSRMLS_DC)
{
	php_dmtx_read_object *intern = (php_dmtx_read_object *)object;
	
	if(!intern) {
		return;
	}
#ifdef HAVE_DMTX_IMAGEMAGICK
	intern->magick_wand = DestroyMagickWand( intern->magick_wand );
#else
	if(intern->image_string) {
		efree(intern->image_string);
	}
#endif
	zend_object_std_dtor(&intern->zo TSRMLS_CC);
	efree(intern);
}

static void php_dmtx_write_object_free_storage(void *object TSRMLS_DC)
{
	php_dmtx_write_object *intern = (php_dmtx_write_object *)object;
	
	if(!intern) {
		return;
	}
#ifdef HAVE_DMTX_IMAGEMAGICK
	intern->magick_wand = DestroyMagickWand( intern->magick_wand );
#endif
	zend_object_std_dtor(&intern->zo TSRMLS_CC);
	efree(intern);
}

static zend_object_value php_dmtx_object_new(zend_class_entry *class_type TSRMLS_DC)
{
	zval *tmp;
	zend_object_value retval;
	php_dmtx_object *intern;
	
	/* Allocate memory for it */
	intern = emalloc(sizeof(php_dmtx_read_object));
	memset(&intern->zo, 0, sizeof(php_dmtx_read_object));
	
	zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
	zend_hash_copy(intern->zo.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref,(void *) &tmp, sizeof(zval *));
	
	retval.handle = zend_objects_store_put(intern, NULL, (zend_objects_free_object_storage_t) php_dmtx_object_free_storage, NULL TSRMLS_CC);
	retval.handlers = (zend_object_handlers *) &php_dmtx_object_handlers;
	return retval;
}

static zend_object_value php_dmtx_read_object_new(zend_class_entry *class_type TSRMLS_DC)
{
	zval *tmp;
	zend_object_value retval;
	php_dmtx_read_object *intern;
	
	/* Allocate memory for it */
	intern = emalloc(sizeof(php_dmtx_read_object));
	memset(&intern->zo, 0, sizeof(php_dmtx_read_object));
#ifdef HAVE_DMTX_IMAGEMAGICK
	intern->magick_wand = NewMagickWand();
#else
	intern->image_string = NULL;
	intern->image_string_len = 0;
#endif
	intern->use_scan_region = 0;
	intern->scan_region_x = 0;
	intern->scan_region_y = 0;
	intern->scan_region_width = 0;
	intern->scan_region_height = 0;

	zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
	zend_hash_copy(intern->zo.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref,(void *) &tmp, sizeof(zval *));
	
	retval.handle = zend_objects_store_put(intern, NULL, (zend_objects_free_object_storage_t) php_dmtx_read_object_free_storage, NULL TSRMLS_CC);
	retval.handlers = (zend_object_handlers *) &php_dmtx_read_object_handlers;
	return retval;
}

static zend_object_value php_dmtx_write_object_new(zend_class_entry *class_type TSRMLS_DC)
{
	zval *tmp;
	zend_object_value retval;
	php_dmtx_write_object *intern;
	
	/* Allocate memory for it */
	intern = emalloc(sizeof(php_dmtx_write_object));
	memset(&intern->zo, 0, sizeof(php_dmtx_write_object));

#ifdef HAVE_DMTX_IMAGEMAGICK
	intern->magick_wand = NewMagickWand();
#endif
	memset( intern->message, '\0', DMTXWRITE_BUFFER_SIZE);
	intern->message_len = 0;

	zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
	zend_hash_copy(intern->zo.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref,(void *) &tmp, sizeof(zval *));
	
	retval.handle = zend_objects_store_put(intern, NULL, (zend_objects_free_object_storage_t) php_dmtx_write_object_free_storage, NULL TSRMLS_CC);
	retval.handlers = (zend_object_handlers *) &php_dmtx_write_object_handlers;
	return retval;
}

PHP_MINIT_FUNCTION(dmtx)
{
#ifdef HAVE_DMTX_IMAGEMAGICK
	MagickWandGenesis();
#endif

	zend_class_entry ce;
	memcpy(&php_dmtx_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	memcpy(&php_dmtx_read_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	memcpy(&php_dmtx_write_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	/*
		Initialize exceptions (dmtx exception)
	*/
	INIT_CLASS_ENTRY(ce, PHP_DMTX_EXCEPTION_SC_NAME, NULL);
	php_dmtx_exception_class_entry = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
	php_dmtx_exception_class_entry->ce_flags |= ZEND_ACC_FINAL;
	
	/*
		Initialize the class (dmtx). This class holds the constants
	*/
	INIT_CLASS_ENTRY(ce, PHP_DMTX_SC_NAME, php_dmtx_class_methods);
	ce.create_object = php_dmtx_object_new;
	php_dmtx_sc_entry = zend_register_internal_class(&ce TSRMLS_CC);

	/* Register constants */
	PHP_DMTX_REGISTER_CONST_LONG("SYMBOL_SQUARE_AUTO", DMTX_SYMBOL_SQUARE_AUTO);
	PHP_DMTX_REGISTER_CONST_LONG("SYMBOL_SQUARE_COUNT", DMTX_SYMBOL_SQUARE_COUNT);
	PHP_DMTX_REGISTER_CONST_LONG("SYMBOL_RECT_AUTO", DMTX_SYMBOL_RECT_AUTO);
	PHP_DMTX_REGISTER_CONST_LONG("SYMBOL_RECT_COUNT", DMTX_SYMBOL_RECT_COUNT);
	/*
		Initialize the class (dmtx read)
	*/
	INIT_CLASS_ENTRY(ce, PHP_DMTX_READ_SC_NAME, php_dmtx_read_class_methods);
	ce.create_object = php_dmtx_read_object_new;
	php_dmtx_read_sc_entry = zend_register_internal_class(&ce TSRMLS_CC);

	/*
		Initialize the class (dmtx write)
	*/
	INIT_CLASS_ENTRY(ce, PHP_DMTX_WRITE_SC_NAME, php_dmtx_write_class_methods);
	ce.create_object = php_dmtx_write_object_new;
	php_dmtx_write_sc_entry = zend_register_internal_class(&ce TSRMLS_CC);

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(dmtx)
{
#ifdef HAVE_DMTX_IMAGEMAGICK
	MagickWandTerminus();
#endif
	return(SUCCESS);
}

PHP_MINFO_FUNCTION(dmtx)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "dmtx module", "enabled");
	php_info_print_table_row(2, "dmtx module version", PHP_DMTX_EXTVER);
	php_info_print_table_end();
}

static function_entry php_dmtx_functions[] =
{
	{ NULL, NULL, NULL }
};

zend_module_entry dmtx_module_entry =
{
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_DMTX_EXTNAME,
	php_dmtx_functions,			/* Functions */
	PHP_MINIT(dmtx),			/* MINIT */
	PHP_MSHUTDOWN(dmtx),		/* MSHUTDOWN */
	NULL,						/* RINIT */
	NULL,						/* RSHUTDOWN */
	PHP_MINFO(dmtx),			/* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
	PHP_DMTX_EXTVER,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_DMTX
ZEND_GET_MODULE(dmtx)
#endif


