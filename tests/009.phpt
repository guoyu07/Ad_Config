--TEST--
return type in Ad_Config_Simple::valid() should be boolean
--SKIPIF--
<?php if (!extension_loaded("ad_config")) print "skip"; ?>
--INI--
--FILE--
<?php

$new = new Ad_Config_Simple(array());
var_dump($new->valid());
?>
--CLEAN--
--EXPECTF--
bool(false)
