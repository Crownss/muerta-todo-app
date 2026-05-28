#!/bin/bash
TARGET=${1:-cli}
rm -rf build && mkdir build && cd build && cmake .. && make && ./muerta_$TARGET