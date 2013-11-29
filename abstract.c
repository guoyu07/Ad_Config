#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "standard/php_filestat.h" /* for php_stat */

#include "php_yaconf.h"
#include "abstract.h"
#include "ini.h"
#include "simple.h"

static zval * yaconf_ini_zval_persistent(zval *zvalue TSRMLS_DC);
static zval * yaconf_ini_zval_losable(zval *zvalue TSRMLS_DC);

zend_class_entry *yaconf_abstract_ce;

/* {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(yaconf_void_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ yaconf_ini_modified
*/
static int yaconf_ini_modified(zval * file, long ctime TSRMLS_DC) {
        zval  n_ctime;
        php_stat(Z_STRVAL_P(file), Z_STRLEN_P(file), 7 /* FS_CTIME */ , &n_ctime TSRMLS_CC);
        if (Z_TYPE(n_ctime) != IS_BOOL && ctime != Z_LVAL(n_ctime)) {
                return Z_LVAL(n_ctime);
        }
        return 0;
}
/* }}} */

/** {{{ static void yaconf_cache_dtor(yaconf_cache **cache)
 */
static void yaconf_cache_dtor(yaconf_cache **cache) {
        if (*cache) {
                zend_hash_destroy((*cache)->data);
                pefree((*cache)->data, 1);
                pefree(*cache, 1);
        }
}
/* }}} */

/** {{{ static void yaconf_zval_dtor(zval **value)
 */
static void yaconf_zval_dtor(zval **value) {
        if (*value) {
                switch(Z_TYPE_PP(value)) {
                        case IS_STRING:
                        case IS_CONSTANT:
                                CHECK_ZVAL_STRING(*value);
                                pefree((*value)->value.str.val, 1);
                                pefree(*value, 1);
                                break;
                        case IS_ARRAY:
                        case IS_CONSTANT_ARRAY: {
                                zend_hash_destroy((*value)->value.ht);
                                pefree((*value)->value.ht, 1);
                                pefree(*value, 1);
                        }
                        break;
                }
        }
}
/* }}} */

/** {{{ static void yaconf_copy_persistent(HashTable *pdst, HashTable *src TSRMLS_DC)
 */
static void yaconf_copy_persistent(HashTable *pdst, HashTable *src TSRMLS_DC) {
        zval **ppzval;
        char *key;
        uint keylen;
        ulong idx;

        for(zend_hash_internal_pointer_reset(src);
                        zend_hash_has_more_elements(src) == SUCCESS;
                        zend_hash_move_forward(src)) {

                if (zend_hash_get_current_key_ex(src, &key, &keylen, &idx, 0, NULL) == HASH_KEY_IS_LONG) {
                        zval *tmp;
                        if (zend_hash_get_current_data(src, (void**)&ppzval) == FAILURE) {
                                continue;
                        }

                        tmp = yaconf_ini_zval_persistent(*ppzval TSRMLS_CC);
                        if (tmp) {
                                zend_hash_index_update(pdst, idx, (void **)&tmp, sizeof(zval *), NULL);
                        }

                } else {
                        zval *tmp;
                        if (zend_hash_get_current_data(src, (void**)&ppzval) == FAILURE) {
                                continue;
                        }

                        tmp = yaconf_ini_zval_persistent(*ppzval TSRMLS_CC);
                        if (tmp) {
                                zend_hash_update(pdst, key, keylen, (void **)&tmp, sizeof(zval *), NULL);
                        }
                }
        }
}
/* }}} */

/** {{{ static void yaconf_copy_losable(HashTable *ldst, HashTable *src TSRMLS_DC)
 */
static void yaconf_copy_losable(HashTable *ldst, HashTable *src TSRMLS_DC) {
        zval **ppzval, *tmp;
        char *key;
        ulong idx;
        uint keylen;

        for(zend_hash_internal_pointer_reset(src);
                        zend_hash_has_more_elements(src) == SUCCESS;
                        zend_hash_move_forward(src)) {

                if (zend_hash_get_current_key_ex(src, &key, &keylen, &idx, 0, NULL) == HASH_KEY_IS_LONG) {
                        if (zend_hash_get_current_data(src, (void**)&ppzval) == FAILURE) {
                                continue;
                        }

                        tmp = yaconf_ini_zval_losable(*ppzval TSRMLS_CC);
                        zend_hash_index_update(ldst, idx, (void **)&tmp, sizeof(zval *), NULL);

                } else {
                        if (zend_hash_get_current_data(src, (void**)&ppzval) == FAILURE) {
                                continue;
                        }

                        tmp = yaconf_ini_zval_losable(*ppzval TSRMLS_CC);
                        zend_hash_update(ldst, key, keylen, (void **)&tmp, sizeof(zval *), NULL);
                }
        }
}
/* }}} */

/** {{{ static zval * yaconf_ini_zval_persistent(zval *zvalue TSRMLS_DC)
 */
static zval * yaconf_ini_zval_persistent(zval *zvalue TSRMLS_DC) {
        zval *ret = (zval *)pemalloc(sizeof(zval), 1);
        INIT_PZVAL(ret);
        switch (zvalue->type) {
                case IS_RESOURCE:
                case IS_OBJECT:
                        break;
                case IS_BOOL:
                case IS_LONG:
                case IS_NULL:
                        break;
                case IS_CONSTANT:
                case IS_STRING:
                                CHECK_ZVAL_STRING(zvalue);
                                Z_TYPE_P(ret) = IS_STRING;
                                ret->value.str.val = pestrndup(zvalue->value.str.val, zvalue->value.str.len, 1);
                                ret->value.str.len = zvalue->value.str.len;
                        break;
                case IS_ARRAY:
                case IS_CONSTANT_ARRAY: {
                                HashTable *tmp_ht, *original_ht = zvalue->value.ht;

                                tmp_ht = (HashTable *)pemalloc(sizeof(HashTable), 1);
                                if (!tmp_ht) {
                                        return NULL;
                                }

                                zend_hash_init(tmp_ht, zend_hash_num_elements(original_ht), NULL, (dtor_func_t)yaconf_zval_dtor, 1);
                                yaconf_copy_persistent(tmp_ht, original_ht TSRMLS_CC);
                                Z_TYPE_P(ret) = IS_ARRAY;
                                ret->value.ht = tmp_ht;
                        }
                        break;
        }

        return ret;
}
/* }}} */

/** {{{ static zval * yaconf_ini_zval_losable(zval *zvalue TSRMLS_DC)
 */
static zval * yaconf_ini_zval_losable(zval *zvalue TSRMLS_DC) {
        zval *ret;
        MAKE_STD_ZVAL(ret);
        switch (zvalue->type) {
                case IS_RESOURCE:
                case IS_OBJECT:
                        break;
                case IS_BOOL:
                case IS_LONG:
                case IS_NULL:
                        break;
                case IS_CONSTANT:
                case IS_STRING:
                        CHECK_ZVAL_STRING(zvalue);
                        ZVAL_STRINGL(ret, zvalue->value.str.val, zvalue->value.str.len, 1);
                        break;
                case IS_ARRAY:
                case IS_CONSTANT_ARRAY: {
                        HashTable *original_ht = zvalue->value.ht;
                        array_init(ret);
                        yaconf_copy_losable(Z_ARRVAL_P(ret), original_ht TSRMLS_CC);
                }
                        break;
        }

        return ret;
}
/* }}} */

/** {{{ static zval * yaconf_ini_unserialize(zval *this_ptr, zval *filename, zval *section TSRMLS_DC)
 */
static zval * yaconf_ini_unserialize(zval *this_ptr, zval *filename, zval *section TSRMLS_DC) {
        char *key;
        uint len;
        yaconf_cache **ppval;

        if (!YACONF_G(configs)) {
                return NULL;
        }

        len = spprintf(&key, 0, "%s#%s", Z_STRVAL_P(filename), Z_STRVAL_P(section));

        if (zend_hash_find(YACONF_G(configs), key, len + 1, (void **)&ppval) == SUCCESS) {
                if (yaconf_ini_modified(filename, (*ppval)->ctime TSRMLS_CC)) {
                        efree(key);
                        return NULL;
                } else {
                        zval *props;

                        MAKE_STD_ZVAL(props);
                        array_init(props);
                        yaconf_copy_losable(Z_ARRVAL_P(props), (*ppval)->data TSRMLS_CC);
                        efree(key);
                        /* tricky way */
                        Z_SET_REFCOUNT_P(props, 0);
                        return yaconf_ini_instance(this_ptr, props, section TSRMLS_CC);
                }
                efree(key);
        }

        return NULL;
}
/* }}} */

/** {{{ static void yaconf_ini_serialize(zval *this_ptr, zval *filename, zval *section TSRMLS_DC)
 */
static void yaconf_ini_serialize(zval *this_ptr, zval *filename, zval *section TSRMLS_DC) {
        char *key;
        uint len;
        long ctime;
        zval *configs;
        HashTable *persistent;
        yaconf_cache *cache;

        if (!YACONF_G(configs)) {
                YACONF_G(configs) = (HashTable *)pemalloc(sizeof(HashTable), 1);
                if (!YACONF_G(configs)) {
                        return;
                }
                zend_hash_init(YACONF_G(configs), 8, NULL, (dtor_func_t) yaconf_cache_dtor, 1);
        }

        cache = (yaconf_cache *)pemalloc(sizeof(yaconf_cache), 1);

        if (!cache) {
                return;
        }

        persistent = (HashTable *)pemalloc(sizeof(HashTable), 1);
        if (!persistent) {
                return;
        }

        configs = zend_read_property(yaconf_ini_ce, this_ptr, ZEND_STRL(YACONF_PROPERT_NAME), 1 TSRMLS_CC);

        zend_hash_init(persistent, zend_hash_num_elements(Z_ARRVAL_P(configs)), NULL, (dtor_func_t) yaconf_zval_dtor, 1);

        yaconf_copy_persistent(persistent, Z_ARRVAL_P(configs) TSRMLS_CC);

        ctime = yaconf_ini_modified(filename, 0 TSRMLS_CC);
        cache->ctime = ctime;
        cache->data  = persistent;
        len = spprintf(&key, 0, "%s#%s", Z_STRVAL_P(filename), Z_STRVAL_P(section));

        zend_hash_update(YACONF_G(configs), key, len + 1, (void **)&cache, sizeof(yaconf_cache *), NULL);

        efree(key);
}
/* }}} */

/** {{{ zval * yaconf_instance(zval *this_ptr, zval *arg1, zval *arg2 TSRMLS_DC)
 */
zval * yaconf_instance(zval *this_ptr, zval *arg1, zval *arg2 TSRMLS_DC) {
        zval *instance;

        if (!arg1) {
                return NULL;
        }

        if (Z_TYPE_P(arg1) == IS_STRING) {
                if (strncasecmp(Z_STRVAL_P(arg1) + Z_STRLEN_P(arg1) - 3, "ini", 3) == 0) {
                        if (YACONF_G(cache_config)) {
                                if ((instance = yaconf_ini_unserialize(this_ptr, arg1, arg2 TSRMLS_CC))) {
                                        return instance;
                                }
                        }

                        instance = yaconf_ini_instance(this_ptr, arg1, arg2 TSRMLS_CC);

                        if (!instance) {
                                return NULL;
                        }

                        if (YACONF_G(cache_config)) {
                                yaconf_ini_serialize(instance, arg1, arg2 TSRMLS_CC);
                        }

                        return instance;
                }
                php_error_docref(NULL TSRMLS_CC, E_ERROR, "Expects a path to *.ini configuration file as parameter");
                return NULL;
        }

        if (Z_TYPE_P(arg1) == IS_ARRAY) {
                zval *readonly;

                MAKE_STD_ZVAL(readonly);
                ZVAL_BOOL(readonly, 1);
                instance = yaconf_simple_instance(this_ptr, arg1, readonly TSRMLS_CC);
                efree(readonly);
                return instance;
        }

        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Expects a string or an array as parameter");
        return NULL;
}
/* }}} */

/** {{{ yaconf_methods
*/
zend_function_entry yaconf_methods[] = {
        PHP_ABSTRACT_ME(yaconf_abstract, get, NULL)
        PHP_ABSTRACT_ME(yaconf_abstract, set, NULL)
        PHP_ABSTRACT_ME(yaconf_abstract, readonly, NULL)
        PHP_ABSTRACT_ME(yaconf_abstract, toArray, NULL)
        {NULL, NULL, NULL}
};
/* }}} */

/** {{{ ZEND_MINIT_FUNCTION
*/
ZEND_MINIT_FUNCTION(yaconf_abstract) {
        zend_class_entry ce;

        INIT_CLASS_ENTRY(ce, "Yaconf_Abstract", yaconf_methods);
        yaconf_abstract_ce = zend_register_internal_class_ex(&ce, NULL, NULL TSRMLS_CC);
        yaconf_abstract_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

        zend_declare_property_null(yaconf_abstract_ce, ZEND_STRL(YACONF_PROPERT_NAME), ZEND_ACC_PROTECTED TSRMLS_CC);
        zend_declare_property_bool(yaconf_abstract_ce, ZEND_STRL(YACONF_PROPERT_NAME_READONLY), 1, ZEND_ACC_PROTECTED TSRMLS_CC);

        ZEND_MODULE_STARTUP_N(yaconf_ini)(INIT_FUNC_ARGS_PASSTHRU);
        ZEND_MODULE_STARTUP_N(yaconf_simple)(INIT_FUNC_ARGS_PASSTHRU);

        return SUCCESS;
}
/* }}} */