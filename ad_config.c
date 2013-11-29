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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_ad_config.h"

#include "abstract.h"


ZEND_DECLARE_MODULE_GLOBALS(ad_config)

/* True global resources - no need for thread safety here */
static int le_yaconf;

/* {{{ yaconf_functions[]
 *
 * Every user visible function must have an entry in yaconf_functions[].
 */
const zend_function_entry ad_config_functions[] = {
        {NULL, NULL, NULL}
};
/* }}} */

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_BOOLEAN("ad_config.cache_config", "0", PHP_INI_SYSTEM, OnUpdateBool, cache_config, zend_ad_config_globals, ad_config_globals)
PHP_INI_END()
/* }}} */

/** {{{ PHP_GINIT_FUNCTION
*/
PHP_GINIT_FUNCTION(ad_config)
{
    ad_config_globals->configs = NULL;
}
/* }}} */


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ad_config)
{
        REGISTER_INI_ENTRIES();

    ZEND_MODULE_STARTUP_N(ad_config_abstract)(INIT_FUNC_ARGS_PASSTHRU);

        return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(ad_config)
{
        UNREGISTER_INI_ENTRIES();

    if (AD_CONFIG_G(configs)) {
        zend_hash_destroy(AD_CONFIG_G(configs));
        pefree(AD_CONFIG_G(configs), 1);
    }

    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(ad_config)
{
        return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(ad_config)
{
        return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(ad_config)
{
        php_info_print_table_start();
        php_info_print_table_header(2, "ad_config support", "enabled");
        php_info_print_table_end();

        DISPLAY_INI_ENTRIES();
}
/* }}} */

#ifdef COMPILE_DL_AD_CONFIG
ZEND_GET_MODULE(ad_config)
#endif

/** {{{ module depends
 */
#if ZEND_MODULE_API_NO >= 20050922
zend_module_dep ad_config_deps[] = {
        ZEND_MOD_REQUIRED("spl")
        ZEND_MOD_REQUIRED("pcre")
        {NULL, NULL, NULL}
};
#endif
/* }}} */

/* {{{ yaconf_module_entry
 */
zend_module_entry ad_config_module_entry = {
#if ZEND_MODULE_API_NO >= 20050922
        STANDARD_MODULE_HEADER_EX, NULL,
        ad_config_deps,
#else
        STANDARD_MODULE_HEADER,
#endif
    "ad_config",
    ad_config_functions,
    PHP_MINIT(ad_config),
    PHP_MSHUTDOWN(ad_config),
    PHP_RINIT(ad_config),      /* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(ad_config),  /* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(ad_config),
#if ZEND_MODULE_API_NO >= 20010901
    "0.1", /* Replace with version number for your extension */
#endif
    PHP_MODULE_GLOBALS(ad_config),
    PHP_GINIT(ad_config),
    NULL,
    NULL,
    STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */