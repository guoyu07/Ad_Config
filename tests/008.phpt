--TEST--
Check for yaconf.system settings
--SKIPIF--
<?php if (!extension_loaded("yaconf")) print "skip"; ?>
--INI--
yaf.cache_config=0
yaf.environ="product"
--FILE--
<?php
define("APPLICATION_PATH", dirname(__FILE__));

var_dump(ini_get("yaf.cache_config") == "0");

new Plugin__ABC_EFG();
?>
--EXPECTF--
bool(true)

Fatal error: Class 'Plugin__ABC_EFG' not found in %s008.php on line %d
