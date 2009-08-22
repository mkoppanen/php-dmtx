--TEST--
Test multiple regions on multiple pages
--SKIPIF--
<?php
include dirname(__FILE__) . "/skipif.inc.php";
?>
--FILE--
<?php
$r = new dmtxread(dirname(__FILE__) . "/multi_region_page.gif");
var_dump($r->getInfo());
?>
--EXPECT--
array(2) {
  [0]=>
  array(2) {
    [0]=>
    array(10) {
      ["message"]=>
      string(5) "Hello"
      ["codewords"]=>
      int(5)
      ["rotation_angle"]=>
      int(357)
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
        string(7) "359x235"
        ["bottom"]=>
        string(7) "365x180"
        ["top"]=>
        string(7) "362x239"
        ["right"]=>
        string(7) "420x181"
      }
      ["bounds"]=>
      array(2) {
        ["bound_min"]=>
        string(7) "359x179"
        ["bound_max"]=>
        string(7) "420x239"
      }
    }
    [1]=>
    array(10) {
      ["message"]=>
      string(5) "Hello"
      ["codewords"]=>
      int(5)
      ["rotation_angle"]=>
      int(4)
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
        string(6) "59x491"
        ["bottom"]=>
        string(6) "67x480"
        ["top"]=>
        string(6) "62x540"
        ["right"]=>
        string(7) "120x481"
      }
      ["bounds"]=>
      array(2) {
        ["bound_min"]=>
        string(6) "59x479"
        ["bound_max"]=>
        string(7) "120x540"
      }
    }
  }
  [1]=>
  array(2) {
    [0]=>
    array(10) {
      ["message"]=>
      string(5) "Hello"
      ["codewords"]=>
      int(5)
      ["rotation_angle"]=>
      int(357)
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
        string(7) "359x235"
        ["bottom"]=>
        string(7) "365x180"
        ["top"]=>
        string(7) "362x239"
        ["right"]=>
        string(7) "420x181"
      }
      ["bounds"]=>
      array(2) {
        ["bound_min"]=>
        string(7) "359x179"
        ["bound_max"]=>
        string(7) "420x239"
      }
    }
    [1]=>
    array(10) {
      ["message"]=>
      string(5) "Hello"
      ["codewords"]=>
      int(5)
      ["rotation_angle"]=>
      int(4)
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
        string(6) "59x491"
        ["bottom"]=>
        string(6) "67x480"
        ["top"]=>
        string(6) "62x540"
        ["right"]=>
        string(7) "120x481"
      }
      ["bounds"]=>
      array(2) {
        ["bound_min"]=>
        string(6) "59x479"
        ["bound_max"]=>
        string(7) "120x540"
      }
    }
  }
}