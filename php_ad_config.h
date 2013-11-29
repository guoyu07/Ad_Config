/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_AD_CONFIG_H
#define PHP_AD_CONFIG_H

extern zend_module_entry ad_config_module_entry;
#define phpext_ad_config_ptr &ad_config_module_entry

#ifdef PHP_WIN32
#	define PHP_AD_CONFIG_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_AD_CONFIG_API __attribute__ ((visibility("default")))
#else
#	define PHP_AD_CONFIG_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION > 2)) || (PHP_MAJOR_VERSION > 5)
#else
#define Z_SET_REFCOUNT_P(pz, rc)      (pz)->refcount = rc
#define Z_SET_REFCOUNT_PP(ppz, rc)    Z_SET_REFCOUNT_P(*(ppz), rc)
#define Z_ADDREF_P          ZVAL_ADDREF
#define Z_REFCOUNT_P ZVAL_REFCOUNT
#define Z_DELREF_P          ZVAL_DELREF
#endif   

ZEND_BEGIN_MODULE_GLOBALS(ad_config)
  zval        *active_ini_file_section;
        zval        *ini_wanted_section;
        uint        parsing_flag;
        zend_bool         cache_config;
        HashTable        *configs;
ZEND_END_MODULE_GLOBALS(ad_config)

extern ZEND_DECLARE_MODULE_GLOBALS(ad_config);


PHP_MINIT_FUNCTION(ad_config);
PHP_MSHUTDOWN_FUNCTION(ad_config);
PHP_RINIT_FUNCTION(ad_config);
PHP_RSHUTDOWN_FUNCTION(ad_config);
PHP_MINFO_FUNCTION(ad_config);



#ifdef ZTS
#define AD_CONFIG_G(v) TSRMG(ad_config_globals_id, zend_ad_config_globals *, v)
#else
#define AD_CONFIG_G(v) (ad_config_globals.v)
#endif

#endif	/* PHP_YACONF_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
