--TEST--
Check for Ad_Config_Ini::__construct with section
--SKIPIF--
<?php if (!extension_loaded("ad_config")) print "skip"; ?>
--FILE--
<?php
$file = dirname(__FILE__) . "/simple.ini";

try {
  $config = new Ad_Config_Ini($file, "ex");
} catch (Exception $e) { 
  print_r($e->getMessage());
}

?>
--EXPECTF--
Fatal error: Ad_Config_Ini::__construct(): There is no section 'ex' in '%ssimple.ini' in %s006.php on line %d