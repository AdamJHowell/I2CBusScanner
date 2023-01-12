cmake_minimum_required( VERSION 3.24 )
project( I2CBusScanner )

set( CMAKE_CXX_STANDARD 14 )

include_directories( . )

add_executable( I2CBusScanner I2CBusScanner.ino )

set_target_properties( I2CBusScanner PROPERTIES LINKER_LANGUAGE CXX )
