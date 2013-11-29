--TEST--
Bug (mem leak and crash in Yaconf_Ini)
--SKIPIF--
<?php if (!extension_loaded("yaconf")) print "skip"; ?>
--FILE--
<?php 
$config = new Yaconf_Ini;

var_dump($config->get("\0"));
?>
--EXPECTF--
Warning: Yaconf_Ini::__construct() expects at least 1 parameter, 0 given in %s on line %d
NULL
