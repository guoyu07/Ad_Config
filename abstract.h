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

#ifndef AD_CONFIG_ABSTRACT_H
#define AD_CONFIG_ABSTRACT_H

#define AD_CONFIG_EXTRACT_FUNC_NAME                        "extract"
#define AD_CONFIG_PROPERT_NAME                      "_config"
#define AD_CONFIG_PROPERT_NAME_READONLY             "_readonly"

struct _ad_config_cache {
        long ctime;
        HashTable *data;
};

typedef struct _ad_config_cache ad_config_cache;

extern zend_class_entry *ad_config_abstract_ce;

zval * ad_config_instance(zval *this_ptr, zval *arg1, zval *arg2 TSRMLS_DC);
void ad_config_unserialize(zval *self, HashTable *data TSRMLS_DC);

#ifndef pestrndup
/* before php-5.2.4, pestrndup is not defined */
#define pestrndup(s, length, persistent) ((persistent)?zend_strndup((s),(length)):estrndup((s),(length)))
#endif

ZEND_MINIT_FUNCTION(ad_config_abstract);
#endif