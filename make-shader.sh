#!/bin/sh

cd $(dirname "$0")

./tools/sokol-tools-bin/bin/linux/sokol-shdc --input ./src/shader/unlit.glsl --output ./src/shader/unlit.glsl.h --slang glsl330
