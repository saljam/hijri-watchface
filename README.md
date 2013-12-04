# Hijri Watchface

An Arabic Hijri calendar watchface for the Pebble. Displays the time, Hijri and Gregorian calendars.

The date calculation is done using what's called a “tabular Islamic calendar”, with the usual 30-year leap cycle on years 2, 5, 7, 10, 13, 16, 18, 21, 24, 26 and 29.

## Building

This uses an [alternative](https://github.com/saljam/pebble) build system for the Pebble.
The prerequisites are:
- $PEBBLE_HOME env var needs to point to the Pebble SDK root.
- elf2pbla in your path (install it using “go get github.com/saljam/pebble/elf2pbla”)

Then just run `make` and you'll get a bundle in bin/hijri.pbw
