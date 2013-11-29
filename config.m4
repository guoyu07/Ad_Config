dnl $Id$
dnl config.m4 for extension ad_config

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(ad_config, for ad_config support,
dnl Make sure that the comment is aligned:
dnl [  --with-ad_config             Include ad_config support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(ad_config, whether to enable ad_config support,
dnl Make sure that the comment is aligned:
[  --enable-ad_config           Enable ad_config support])

if test "$PHP_YACONF" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-ad_config -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/ad_config.h"  # you most likely want to change this
  dnl if test -r $PHP_YACONF/$SEARCH_FOR; then # path given as parameter
  dnl   YACONF_DIR=$PHP_YACONF
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for ad_config files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       YACONF_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$YACONF_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the ad_config distribution])
  dnl fi

  dnl # --with-ad_config -> add include path
  dnl PHP_ADD_INCLUDE($YACONF_DIR/include)

  dnl # --with-ad_config -> check for lib and symbol presence
  dnl LIBNAME=ad_config # you may want to change this
  dnl LIBSYMBOL=ad_config # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $YACONF_DIR/lib, YACONF_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_YACONFLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong ad_config lib version or lib not found])
  dnl ],[
  dnl   -L$YACONF_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(YACONF_SHARED_LIBADD)

  PHP_NEW_EXTENSION(ad_config, ad_config.c abstract.c ini.c simple.c, $ext_shared)
fi
