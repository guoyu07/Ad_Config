--TEST--
return type in Yaconf_Simple::valid() should be boolean
--SKIPIF--
<?php if (!extension_loaded("yaconf")) print "skip"; ?>
--INI--
--FILE--
<?php

$new = new Yaconf_Simple(array());
var_dump($new->valid());
?>
--CLEAN--
--EXPECTF--
bool(false)
