#!/bin/sh

export ASAN_OPTIONS=detect_leaks=0 && meson compile -C build && ./build/lbtl
