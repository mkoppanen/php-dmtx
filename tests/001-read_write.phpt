--TEST--
Test reading and writing
--SKIPIF--
<?php
include dirname(__FILE__) . "/skipif.inc.php";
?>
--FILE--
<?php
$w = new dmtxWrite("Hello"); 
$w->save("test.png", dmtx::SYMBOL_SQUARE_AUTO);

$r = new dmtxRead("test.png");
var_dump($r->getInfo());

?>
--EXPECT--
array(1) {
  [0]=>
  array(1) {
    [0]=>
    array(10) {
      ["message"]=>
      string(5) "Hello"
      ["codewords"]=>
      int(5)
      ["rotation_angle"]=>
      int(0)
      ["matrix_width"]=>
      int(14)
      ["matrix_height"]=>
      int(14)
      ["data_regions_horizontal"]=>
      int(1)
      ["data_regions_vertical"]=>
      int(1)
      ["interleaved_blocks"]=>
      int(1)
      ["edges"]=>
      array(4) {
        ["left"]=>
        string(4) "9x44"
        ["bottom"]=>
        string(5) "17x10"
        ["top"]=>
        string(5) "10x79"
        ["right"]=>
        string(5) "79x10"
      }
      ["bounds"]=>
      array(2) {
        ["bound_min"]=>
        string(4) "9x10"
        ["bound_max"]=>
        string(5) "79x79"
      }
    }
  }
}