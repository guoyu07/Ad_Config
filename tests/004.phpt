--TEST--
Bug (mem leak and crash in Ad_Config_Ini)
--SKIPIF--
<?php if (!extension_loaded("ad_config")) print "skip"; ?>
--FILE--
<?php 
$config = new Ad_Config_Ini;

var_dump($config->get("\0"));
?>
--EXPECTF--
Warning: Ad_Config_Ini::__construct() expects at least 1 parameter, 0 given in %s on line %d
NULL
