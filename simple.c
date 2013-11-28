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
#include "Zend/zend_interfaces.h"

#include "php_yaconf.h"

#include "simple.h"

zend_class_entry *yaconf_simple_ce;

#ifdef HAVE_SPL
extern PHPAPI zend_class_entry *spl_ce_Countable;
#endif

/** {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(yaconf_simple_void_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yaconf_simple_construct_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, config_file)
    ZEND_ARG_INFO(0, section)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yaconf_simple_get_arginfo, 0, 0, 0)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yaconf_simple_rget_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yaconf_simple_set_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, name)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yaconf_simple_isset_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yaconf_simple_unset_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ zval * yaconf_simple_instance(zval *this_ptr, zval *values, zval *readonly TSRMLS_DC)
*/
zval * yaconf_simple_instance(zval *this_ptr, zval *values, zval *readonly TSRMLS_DC) {
    zval *instance;

    switch (Z_TYPE_P(values)) {
        case IS_ARRAY:
            if (this_ptr) {
                instance = this_ptr;
            } else {
                MAKE_STD_ZVAL(instance);
                object_init_ex(instance, yaconf_simple_ce);
            }
            zend_update_property(yaconf_simple_ce, instance, ZEND_STRL(YACONF_PROPERT_NAME), values TSRMLS_CC);
            if (readonly) {
                convert_to_boolean(readonly);
                zend_update_property_bool(yaconf_simple_ce, instance, ZEND_STRL(YACONF_PROPERT_NAME_READONLY), Z_BVAL_P(readonly) TSRMLS_CC);
            }
            return instance;
        break;
        default:
            yaf_trigger_error(YAF_ERR_TYPE_ERROR TSRMLS_CC, "Invalid parameters provided, must be an array");
            return NULL;
    }
}
/* }}} */

/** {{{ zval * yaconf_simple_format(zval *instance, zval **ppzval TSRMLS_DC)
 */
zval * yaconf_simple_format(zval *instance, zval **ppzval TSRMLS_DC) {
    zval *readonly, *ret;
    readonly = zend_read_property(yaconf_simple_ce, instance, ZEND_STRL(YACONF_PROPERT_NAME_READONLY), 1 TSRMLS_CC);
    ret = yaconf_simple_instance(NULL, *ppzval, readonly TSRMLS_CC);
    return ret;
}
/* }}} */

/** {{{ proto public Yaconf_Simple::__construct(mixed $array, string $readonly)
*/
PHP_METHOD(yaconf_simple, __construct) {
    zval *values, *readonly = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &values, &readonly) == FAILURE) {
        zval *prop;

        MAKE_STD_ZVAL(prop);
        array_init(prop);
        zend_update_property(yaconf_simple_ce, getThis(), ZEND_STRL(YACONF_PROPERT_NAME), prop TSRMLS_CC);
        zval_ptr_dtor(&prop);

        return;
    }

    (void)yaf_config_simple_instance(getThis(), values, readonly TSRMLS_CC);
}
/** }}} */

/** {{{ proto public Yaconf_Simple::get(string $name = NULL)
*/
PHP_METHOD(yaconf_simple, get) {
    zval *ret, **ppzval;
    char *name;
    uint len = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &name, &len) == FAILURE) {
        return;
    }

    if (!len) {
        RETURN_ZVAL(getThis(), 1, 0);
    } else {
        zval *properties;
        HashTable *hash;

        properties = zend_read_property(yaconf_simple_ce, getThis(), ZEND_STRL(YACONF_PROPERT_NAME), 1 TSRMLS_CC);
        hash  = Z_ARRVAL_P(properties);

        if (zend_hash_find(hash, name, len + 1, (void **) &ppzval) == FAILURE) {
            RETURN_FALSE;
        }

        if (Z_TYPE_PP(ppzval) == IS_ARRAY) {
            if ((ret = yaconf_simple_format(getThis(), ppzval TSRMLS_CC))) {
                RETURN_ZVAL(ret, 1, 1);
            } else {
                RETURN_NULL();
            }
        } else {
            RETURN_ZVAL(*ppzval, 1, 0);
        }
    }

    RETURN_FALSE;
}
/* }}} */

/** {{{ proto public Yaconf_Simple::toArray(void)
*/
PHP_METHOD(yaconf_simple, toArray) {
    zval *properties = zend_read_property(yaconf_simple_ce, getThis(), ZEND_STRL(YACONF_PROPERT_NAME), 1 TSRMLS_CC);
    RETURN_ZVAL(properties, 1, 0);
}
/* }}} */

/** {{{ proto public Yaconf_Simple::set($name, $value)
*/
PHP_METHOD(yaconf_simple, set) {
    zval *readonly = zend_read_property(yaconf_simple_ce, getThis(), ZEND_STRL(YACONF_PROPERT_NAME_READONLY), 1 TSRMLS_CC);

    if (!Z_BVAL_P(readonly)) {
        zval *name, *value, *props;
        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &name, &value) == FAILURE) {
            return;
        }

        if (Z_TYPE_P(name) != IS_STRING || !Z_STRLEN_P(name)) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "Expect a string key name");
            RETURN_FALSE;
        }

        Z_ADDREF_P(value);
        props = zend_read_property(yaconf_simple_ce, getThis(), ZEND_STRL(YACONF_PROPERT_NAME), 1 TSRMLS_CC);
        if (zend_hash_update(Z_ARRVAL_P(props), Z_STRVAL_P(name), Z_STRLEN_P(name) + 1, (void **)&value, sizeof(zval*), NULL) == SUCCESS) {
            RETURN_TRUE;
        } else {
            Z_DELREF_P(value);
        }
    }

    RETURN_FALSE;
}
/* }}} */

/** {{{ proto public Yaconf_Simple::__isset($name)
*/
PHP_METHOD(yaconf_simple, __isset) {
    char *name;
    uint len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &len) == FAILURE) {
        return;
    } else {
        zval *prop = zend_read_property(yaconf_simple_ce, getThis(), ZEND_STRL(YACONF_PROPERT_NAME), 1 TSRMLS_CC);
        RETURN_BOOL(zend_hash_exists(Z_ARRVAL_P(prop), name, len + 1));
    }
}
/* }}} */

/** {{{ proto public Yaconf_Simple::offsetUnset($index)
*/
PHP_METHOD(yaconf_simple, offsetUnset) {
    zval *readonly = zend_read_property(yaconf_simple_ce, getThis(), ZEND_STRL(YACONF_PROPERT_NAME_READONLY), 1 TSRMLS_CC);

    if (!Z_BVAL_P(readonly)) {
        zval *name, *props;
        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &name) == FAILURE) {
            return;
        }

        if (Z_TYPE_P(name) != IS_STRING || !Z_STRLEN_P(name)) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "Expect a string key name");
            RETURN_FALSE;
        }

        props = zend_read_property(yaconf_simple_ce, getThis(), ZEND_STRL(YACONF_PROPERT_NAME), 1 TSRMLS_CC);
        if (zend_hash_del(Z_ARRVAL_P(props), Z_STRVAL_P(name), Z_STRLEN_P(name) + 1) == SUCCESS) {
            RETURN_TRUE;
        }
    }

    RETURN_FALSE;
}
/* }}} */

/** {{{ proto public Yaconf_Simple::count($name)
*/
PHP_METHOD(yaconf_simple, count) {
    zval *prop = zend_read_property(yaconf_simple_ce, getThis(), ZEND_STRL(YACONF_PROPERT_NAME), 1 TSRMLS_CC);
    RETURN_LONG(zend_hash_num_elements(Z_ARRVAL_P(prop)));
}
/* }}} */

/** {{{ proto public Yaconf_Simple::rewind(void)
*/
PHP_METHOD(yaconf_simple, rewind) {
    zval *prop = zend_read_property(yaconf_simple_ce, getThis(), ZEND_STRL(YACONF_PROPERT_NAME), 1 TSRMLS_CC);
    zend_hash_internal_pointer_reset(Z_ARRVAL_P(prop));
}
/* }}} */

/** {{{ proto public Yaconf_Simple::current(void)
*/
PHP_METHOD(yaconf_simple, current) {
    zval *prop, **ppzval, *ret;

    prop = zend_read_property(yaconf_simple_ce, getThis(), ZEND_STRL(YACONF_PROPERT_NAME), 1 TSRMLS_CC);
    if (zend_hash_get_current_data(Z_ARRVAL_P(prop), (void **)&ppzval) == FAILURE) {
        RETURN_FALSE;
    }

    if (Z_TYPE_PP(ppzval) == IS_ARRAY) {
        if ((ret = yaconf_simple_format(getThis(), ppzval TSRMLS_CC))) {
            RETURN_ZVAL(ret, 1, 1);
        } else {
            RETURN_NULL();
        }
    } else {
        RETURN_ZVAL(*ppzval, 1, 0);
    }
}
/* }}} */

/** {{{ proto public Yaconf_Simple::key(void)
*/
PHP_METHOD(yaconf_simple, key) {
    zval *prop;
    char *string;
    ulong index;

    prop = zend_read_property(yaconf_simple_ce, getThis(), ZEND_STRL(YACONF_PROPERT_NAME), 1 TSRMLS_CC);
    zend_hash_get_current_key(Z_ARRVAL_P(prop), &string, &index, 0);
    switch(zend_hash_get_current_key_type(Z_ARRVAL_P(prop))) {
        case HASH_KEY_IS_LONG:
            RETURN_LONG(index);
            break;
        case HASH_KEY_IS_STRING:
            RETURN_STRING(string, 1);
            break;
        default:
            RETURN_FALSE;
    }
}
/* }}} */

/** {{{ proto public Yaconf_Simple::next(void)
*/
PHP_METHOD(yaconf_simple, next) {
    zval *prop = zend_read_property(yaconf_simple_ce, getThis(), ZEND_STRL(YACONF_PROPERT_NAME), 1 TSRMLS_CC);
    zend_hash_move_forward(Z_ARRVAL_P(prop));
    RETURN_TRUE;
}
/* }}} */

/** {{{ proto public Yaconf_Simple::valid(void)
*/
PHP_METHOD(yaconf_simple, valid) {
    zval *prop = zend_read_property(yaconf_simple_ce, getThis(), ZEND_STRL(YACONF_PROPERT_NAME), 1 TSRMLS_CC);
    RETURN_BOOL(zend_hash_has_more_elements(Z_ARRVAL_P(prop)) == SUCCESS);
}
/* }}} */

/** {{{ proto public Yaconf_Simple::readonly(void)
*/
PHP_METHOD(yaconf_simple, readonly) {
    zval *readonly = zend_read_property(yaconf_simple_ce, getThis(), ZEND_STRL(YACONF_PROPERT_NAME_READONLY), 1 TSRMLS_CC);
    RETURN_BOOL(Z_LVAL_P(readonly));
}
/* }}} */

/** {{{ proto public Yaconf_Simple::__destruct
*/
PHP_METHOD(yaconf_simple, __destruct) {
}
/* }}} */

/** {{{ proto private Yaconf_Simple::__clone
*/
PHP_METHOD(yaconf_simple, __clone) {
}
/* }}} */

/** {{{ yaconf_simple_methods
*/
zend_function_entry yaconf_simple_methods[] = {
    PHP_ME(yaconf_simple, __construct,  yaconf_simple_construct_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    PHP_ME(yaconf_simple, __isset,      yaconf_simple_isset_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(yaconf_simple, get,          yaconf_simple_get_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(yaconf_simple, set,          yaconf_simple_set_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(yaconf_simple, count,        yaconf_simple_void_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(yaconf_simple, offsetUnset,  yaconf_simple_unset_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(yaconf_simple, rewind,       yaconf_simple_void_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(yaconf_simple, current,      yaconf_simple_void_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(yaconf_simple, next,         yaconf_simple_void_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(yaconf_simple, valid,        yaconf_simple_void_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(yaconf_simple, key,          yaconf_simple_void_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(yaconf_simple, readonly,     yaconf_simple_void_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(yaconf_simple, toArray,      yaconf_simple_void_arginfo, ZEND_ACC_PUBLIC)
    PHP_MALIAS(yaconf_simple, __set, set, yaconf_simple_set_arginfo, ZEND_ACC_PUBLIC)
    PHP_MALIAS(yaconf_simple, __get, get, yaconf_simple_get_arginfo, ZEND_ACC_PUBLIC)
    PHP_MALIAS(yaconf_simple, offsetGet, get, yaconf_simple_rget_arginfo, ZEND_ACC_PUBLIC)
    PHP_MALIAS(yaconf_simple, offsetExists, __isset, yaconf_simple_isset_arginfo, ZEND_ACC_PUBLIC)
    PHP_MALIAS(yaconf_simple, offsetSet, set, yaconf_simple_set_arginfo, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};
/* }}} */

/** {{{ ZEND_MINIT_FUNCTION
*/
ZEND_MINIT_FUNCTION(yaconf_simple) {
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "Yaconf_Simple", yaconf_simple_methods);
    yaconf_simple_ce = zend_register_internal_class_ex(&ce, yaconf_ce, NULL TSRMLS_CC);

#ifdef HAVE_SPL
    zend_class_implements(yaconf_simple_ce TSRMLS_CC, 3, zend_ce_iterator, zend_ce_arrayaccess, spl_ce_Countable);
#else
    zend_class_implements(yaconf_simple_ce TSRMLS_CC, 2, zend_ce_iterator, zend_ce_arrayaccess);
#endif
    zend_declare_property_bool(yaconf_simple_ce, ZEND_STRL(YACONF_PROPERT_NAME_READONLY), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

    yaconf_simple_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;

    return SUCCESS;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
