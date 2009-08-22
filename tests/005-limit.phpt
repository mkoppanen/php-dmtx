--TEST--
Test start and limit options
--SKIPIF--
<?php
include dirname(__FILE__) . "/skipif.inc.php";
?>
--FILE--
<?php
echo "---------------------------------------------------------------------------\n";

$r = new dmtxread(dirname(__FILE__) . "/ten_page.gif");
$r->setLimit(0, 2);
var_dump($r->getInfo());

echo "---------------------------------------------------------------------------\n";

$r = new dmtxread(dirname(__FILE__) . "/ten_page.gif");
$r->setLimit(1, 3);
var_dump($r->getInfo());

echo "---------------------------------------------------------------------------\n";

$r = new dmtxread(dirname(__FILE__) . "/ten_page.gif");
$r->setLimit(2, 4);
var_dump($r->getInfo());

echo "---------------------------------------------------------------------------\n";

?>
--EXPECT--
---------------------------------------------------------------------------
array(2) {
  [0]=>
  array(1) {
    [0]=>
    array(10) {
      ["message"]=>
      string(6) "page 0"
      ["codewords"]=>
      int(6)
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
        string(4) "16x9"
        ["top"]=>
        string(5) "10x79"
        ["right"]=>
        string(5) "79x10"
      }
      ["bounds"]=>
      array(2) {
        ["bound_min"]=>
        string(3) "9x9"
        ["bound_max"]=>
        string(5) "79x80"
      }
    }
  }
  [1]=>
  array(1) {
    [0]=>
    array(10) {
      ["message"]=>
      string(6) "page 1"
      ["codewords"]=>
      int(6)
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
        string(4) "16x9"
        ["top"]=>
        string(5) "10x79"
        ["right"]=>
        string(5) "79x10"
      }
      ["bounds"]=>
      array(2) {
        ["bound_min"]=>
        string(3) "9x9"
        ["bound_max"]=>
        string(5) "80x80"
      }
    }
  }
}
---------------------------------------------------------------------------
array(3) {
  [1]=>
  array(1) {
    [0]=>
    array(10) {
      ["message"]=>
      string(6) "page 1"
      ["codewords"]=>
      int(6)
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
        string(4) "16x9"
        ["top"]=>
        string(5) "10x79"
        ["right"]=>
        string(5) "79x10"
      }
      ["bounds"]=>
      array(2) {
        ["bound_min"]=>
        string(3) "9x9"
        ["bound_max"]=>
        string(5) "80x80"
      }
    }
  }
  [2]=>
  array(1) {
    [0]=>
    array(10) {
      ["message"]=>
      string(6) "page 2"
      ["codewords"]=>
      int(6)
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
        string(4) "16x9"
        ["top"]=>
        string(5) "10x79"
        ["right"]=>
        string(5) "79x10"
      }
      ["bounds"]=>
      array(2) {
        ["bound_min"]=>
        string(3) "9x9"
        ["bound_max"]=>
        string(5) "80x80"
      }
    }
  }
  [3]=>
  array(1) {
    [0]=>
    array(10) {
      ["message"]=>
      string(6) "page 3"
      ["codewords"]=>
      int(6)
      ["rotation_angle"]=>
      int(359)
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
        string(4) "16x9"
        ["top"]=>
        string(5) "10x79"
        ["right"]=>
        string(5) "79x10"
      }
      ["bounds"]=>
      array(2) {
        ["bound_min"]=>
        string(3) "9x9"
        ["bound_max"]=>
        string(5) "80x80"
      }
    }
  }
}
---------------------------------------------------------------------------
array(4) {
  [2]=>
  array(1) {
    [0]=>
    array(10) {
      ["message"]=>
      string(6) "page 2"
      ["codewords"]=>
      int(6)
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
        string(4) "16x9"
        ["top"]=>
        string(5) "10x79"
        ["right"]=>
        string(5) "79x10"
      }
      ["bounds"]=>
      array(2) {
        ["bound_min"]=>
        string(3) "9x9"
        ["bound_max"]=>
        string(5) "80x80"
      }
    }
  }
  [3]=>
  array(1) {
    [0]=>
    array(10) {
      ["message"]=>
      string(6) "page 3"
      ["codewords"]=>
      int(6)
      ["rotation_angle"]=>
      int(359)
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
        string(4) "16x9"
        ["top"]=>
        string(5) "10x79"
        ["right"]=>
        string(5) "79x10"
      }
      ["bounds"]=>
      array(2) {
        ["bound_min"]=>
        string(3) "9x9"
        ["bound_max"]=>
        string(5) "80x80"
      }
    }
  }
  [4]=>
  array(1) {
    [0]=>
    array(10) {
      ["message"]=>
      string(6) "page 4"
      ["codewords"]=>
      int(6)
      ["rotation_angle"]=>
      int(359)
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
        string(4) "16x9"
        ["top"]=>
        string(5) "10x79"
        ["right"]=>
        string(5) "79x10"
      }
      ["bounds"]=>
      array(2) {
        ["bound_min"]=>
        string(3) "9x9"
        ["bound_max"]=>
        string(5) "80x80"
      }
    }
  }
  [5]=>
  array(1) {
    [0]=>
    array(10) {
      ["message"]=>
      string(6) "page 5"
      ["codewords"]=>
      int(6)
      ["rotation_angle"]=>
      int(359)
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
        string(4) "16x9"
        ["top"]=>
        string(5) "10x79"
        ["right"]=>
        string(5) "79x10"
      }
      ["bounds"]=>
      array(2) {
        ["bound_min"]=>
        string(3) "9x9"
        ["bound_max"]=>
        string(5) "80x80"
      }
    }
  }
}
---------------------------------------------------------------------------