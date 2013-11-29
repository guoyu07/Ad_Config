--TEST--
Check for Yaconf_Ini
--SKIPIF--
<?php if (!extension_loaded("yaconf")) print "skip"; ?>
--FILE--
<?php 
$file = dirname(__FILE__) . "/simple.ini";

$config = new Yaconf_Ini($file);
print_r($config);
$config = new Yaconf_Ini($file, "extra");
print_r($config);
$config = new Yaconf_Ini($file);
$config->longtime = 23424234324;
var_dump($config->readonly());

foreach($config as $key => $value) {
    print_r($key);
}

$sick = @new Yaconf_Ini();

var_dump($sick->__isset(1));
var_dump($sick->__get(1));
$sick->total = 1;

var_dump(count($sick));
?>
--EXPECTF--
Yaconf_Ini Object
(
    [_config:protected] => Array
        (
            [base] => Array
                (
                    [application] => Array
                        (
                            [directory] => APPLICATION_PATH/applcation
                        )

                    [name] => base
                    [array] => Array
                        (
                            [1] => 1
                            [name] => name
                        )

                    [5] => 5
                    [routes] => Array
                        (
                            [regex] => Array
                                (
                                    [type] => regex
                                    [match] => ^/ap/(.*)
                                    [route] => Array
                                        (
                                            [controller] => Index
                                            [action] => action
                                        )

                                    [map] => Array
                                        (
                                            [0] => name
                                            [1] => name
                                            [2] => value
                                        )

                                )

                            [simple] => Array
                                (
                                    [type] => simple
                                    [controller] => c
                                    [module] => m
                                    [action] => a
                                )

                            [supervar] => Array
                                (
                                    [type] => supervar
                                    [varname] => c
                                )

                            [rewrite] => Array
                                (
                                    [type] => rewrite
                                    [match] => /yaf/:name/:value
                                    [route] => Array
                                        (
                                            [controller] => Index
                                            [action] => action
                                        )

                                )

                        )

                )

            [extra] => Array
                (
                    [application] => Array
                        (
                            [directory] => APPLICATION_PATH/applcation
                        )

                    [name] => extra
                    [array] => Array
                        (
                            [1] => 1
                            [name] => new_name
                            [2] => test
                        )

                    [5] => 5
                    [routes] => Array
                        (
                            [regex] => Array
                                (
                                    [type] => regex
                                    [match] => ^/ap/(.*)
                                    [route] => Array
                                        (
                                            [controller] => Index
                                            [action] => action
                                        )

                                    [map] => Array
                                        (
                                            [0] => name
                                            [1] => name
                                            [2] => value
                                        )

                                )

                            [simple] => Array
                                (
                                    [type] => simple
                                    [controller] => c
                                    [module] => m
                                    [action] => a
                                )

                            [supervar] => Array
                                (
                                    [type] => supervar
                                    [varname] => c
                                )

                            [rewrite] => Array
                                (
                                    [type] => rewrite
                                    [match] => /yaf/:name/:value
                                    [route] => Array
                                        (
                                            [controller] => Index
                                            [action] => action
                                        )

                                )

                        )

                    [value] => 2
                )

            [product] => Array
                (
                    [application] => Array
                        (
                            [directory] => APPLICATION_PATH/applcation
                        )

                    [name] => extra
                    [array] => Array
                        (
                            [1] => 1
                            [name] => new_name
                            [2] => test
                        )

                    [5] => 5
                    [routes] => Array
                        (
                            [regex] => Array
                                (
                                    [type] => regex
                                    [match] => ^/ap/(.*)
                                    [route] => Array
                                        (
                                            [controller] => Index
                                            [action] => action
                                        )

                                    [map] => Array
                                        (
                                            [0] => name
                                            [1] => name
                                            [2] => value
                                        )

                                )

                            [simple] => Array
                                (
                                    [type] => simple
                                    [controller] => c
                                    [module] => m
                                    [action] => a
                                )

                            [supervar] => Array
                                (
                                    [type] => supervar
                                    [varname] => c
                                )

                            [rewrite] => Array
                                (
                                    [type] => rewrite
                                    [match] => /yaf/:name/:value
                                    [route] => Array
                                        (
                                            [controller] => Index
                                            [action] => action
                                        )

                                )

                        )

                    [value] => 2
                )

            [nocatch] => Array
                (
                    [application] => Array
                        (
                            [directory] => APPLICATION_PATH/applcation
                            [dispatcher] => Array
                                (
                                    [throwException] => 
                                    [catchException] => 1
                                )

                        )

                    [name] => extra
                    [array] => Array
                        (
                            [1] => 1
                            [name] => new_name
                            [2] => test
                        )

                    [5] => 5
                    [routes] => Array
                        (
                            [regex] => Array
                                (
                                    [type] => regex
                                    [match] => ^/ap/(.*)
                                    [route] => Array
                                        (
                                            [controller] => Index
                                            [action] => action
                                        )

                                    [map] => Array
                                        (
                                            [0] => name
                                            [1] => name
                                            [2] => value
                                        )

                                )

                            [simple] => Array
                                (
                                    [type] => simple
                                    [controller] => c
                                    [module] => m
                                    [action] => a
                                )

                            [supervar] => Array
                                (
                                    [type] => supervar
                                    [varname] => c
                                )

                            [rewrite] => Array
                                (
                                    [type] => rewrite
                                    [match] => /yaf/:name/:age
                                    [route] => Array
                                        (
                                            [controller] => Index
                                            [action] => action
                                        )

                                )

                        )

                    [value] => 2
                )

            [envtest] => Array
                (
                    [env] => 
                    [ini] => 
                    [const] => FOO
                )

        )

    [_readonly:protected] => 1
)
Yaconf_Ini Object
(
    [_config:protected] => Array
        (
            [application] => Array
                (
                    [directory] => APPLICATION_PATH/applcation
                )

            [name] => extra
            [array] => Array
                (
                    [1] => 1
                    [name] => new_name
                    [2] => test
                )

            [5] => 5
            [routes] => Array
                (
                    [regex] => Array
                        (
                            [type] => regex
                            [match] => ^/ap/(.*)
                            [route] => Array
                                (
                                    [controller] => Index
                                    [action] => action
                                )

                            [map] => Array
                                (
                                    [0] => name
                                    [1] => name
                                    [2] => value
                                )

                        )

                    [simple] => Array
                        (
                            [type] => simple
                            [controller] => c
                            [module] => m
                            [action] => a
                        )

                    [supervar] => Array
                        (
                            [type] => supervar
                            [varname] => c
                        )

                    [rewrite] => Array
                        (
                            [type] => rewrite
                            [match] => /yaf/:name/:value
                            [route] => Array
                                (
                                    [controller] => Index
                                    [action] => action
                                )

                        )

                )

            [value] => 2
        )

    [_readonly:protected] => 1
)
bool(true)
baseextraproductnocatchenvtestbool(false)
NULL
int(0)
