#!/bin/sh

meson setup build --warnlevel 2 -Db_sanitize=address -Db_lundef=false --reconfigure
