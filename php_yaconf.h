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

#ifndef PHP_YACONF_H
#define PHP_YACONF_H

extern zend_module_entry yaconf_module_entry;
#define phpext_yaconf_ptr &yaconf_module_entry

#ifdef PHP_WIN32
#	define PHP_YACONF_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_YACONF_API __attribute__ ((visibility("default")))
#else
#	define PHP_YACONF_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define YACONF_EXTRACT_FUNC_NAME                        "extract"
#define YACONF_CONFIG_PROPERT_NAME                      "_config"
#define YACONF_CONFIG_PROPERT_NAME_READONLY             "_readonly"

#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION < 3)) || (PHP_MAJOR_VERSION < 5)
  #define Z_SET_REFCOUNT_P(pz, rc)      (pz)->refcount = rc
  #define Z_SET_REFCOUNT_PP(ppz, rc)    Z_SET_REFCOUNT_P(*(ppz), rc)
  #define Z_ADDREF_P                    ZVAL_ADDREF
  #define Z_REFCOUNT_P                  ZVAL_REFCOUNT
  #define Z_DELREF_P                    ZVAL_DELREF
#endif

struct _yaconf_config_cache {
        long ctime;
        HashTable *data;
};

typedef struct _yaconf_config_cache yaconf_config_cache;

extern zend_class_entry *yaf_config_ce;

yaf_config_t * yaconf_instance(zval *this_ptr, zval *arg1, zval *arg2 TSRMLS_DC);
void yaconf_unserialize(zval *self, HashTable *data TSRMLS_DC);

#ifndef pestrndup
/* before php-5.2.4, pestrndup is not defined */
#define pestrndup(s, length, persistent) ((persistent)?zend_strndup((s),(length)):estrndup((s),(length)))
#endif

ZEND_MINIT_FUNCTION(yaconf);



PHP_MINIT_FUNCTION(yaconf);
PHP_MSHUTDOWN_FUNCTION(yaconf);
PHP_RINIT_FUNCTION(yaconf);
PHP_RSHUTDOWN_FUNCTION(yaconf);
PHP_MINFO_FUNCTION(yaconf);
  

ZEND_BEGIN_MODULE_GLOBALS(yaconf)
	HashTable        *configs;
  zend_bool        cache_config;
  zval        *active_ini_file_section;
        zval        *ini_wanted_section;
        uint        parsing_flag;
ZEND_END_MODULE_GLOBALS(yaconf)

/* In every utility function you add that needs to use variables 
   in php_yaconf_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as YACONF_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define YACONF_G(v) TSRMG(yaconf_globals_id, zend_yaconf_globals *, v)
#else
#define YACONF_G(v) (yaconf_globals.v)
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
