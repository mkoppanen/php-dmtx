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
      int(359)
      ["matrix_width"]=>
      int(12)
      ["matrix_height"]=>
      int(12)
      ["data_regions_horizontal"]=>
      int(1)
      ["data_regions_vertical"]=>
      int(1)
      ["interleaved_blocks"]=>
      int(1)
      ["edges"]=>
      array(4) {
        ["left"]=>
        string(4) "9x39"
        ["bottom"]=>
        string(5) "16x10"
        ["top"]=>
        string(5) "10x69"
        ["right"]=>
        string(5) "69x10"
      }
      ["bounds"]=>
      array(2) {
        ["bound_min"]=>
        string(4) "9x10"
        ["bound_max"]=>
        string(5) "69x69"
      }
    }
  }
}