--TEST--
Check for Ad_Config_Ini with env
--SKIPIF--
<?php 
if (!extension_loaded("ad_config")) print "skip"; 
if (substr(PHP_OS, 0, 3) == 'WIN') {
    die('skip seems windows doesnt support this');
}
?>
--FILE--
<?php 
putenv("FOO=bar");
define("FOO", "Dummy");
print_r(new Ad_Config_Ini(
    dirname(__FILE__) . "/simple.ini", "envtest"));
?>
--EXPECTF--
Ad_Config_Ini Object
(
    [_config:protected] => Array
        (
            [env] => bar
            [ini] => bar
            [const] => Dummy
        )

    [_readonly:protected] => 1
)
