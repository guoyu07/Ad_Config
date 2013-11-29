--TEST--
Check for Yaconf_Ini with env
--SKIPIF--
<?php 
if (!extension_loaded("yaconf")) print "skip"; 
if (substr(PHP_OS, 0, 3) == 'WIN') {
    die('skip seems windows doesnt support this');
}
?>
--FILE--
<?php 
putenv("FOO=bar");
define("FOO", "Dummy");
print_r(new Yaconf_Ini(
    dirname(__FILE__) . "/simple.ini", "envtest"));
?>
--EXPECTF--
Yaconf_Ini Object
(
    [_config:protected] => Array
        (
            [env] => bar
            [ini] => bar
            [const] => Dummy
        )

    [_readonly:protected] => 1
)
