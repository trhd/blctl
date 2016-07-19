[![Build Status](https://travis-ci.org/trhd/blctl.svg)](https://travis-ci.org/trhd/blctl)

blctl
=====


About
-----

Blctl is a small utility for controlling (e.g. a laptop's) backlight's
brightness.


Building blctl
--------------

Blctl supports Meson build tool.

The specific backlight it controls can be specified during compilation
(see mesonconf's manual page).

Consider setting SUID for the binary during installation.


Usage
-----

	$ blctl -h
	Usage: blctl [OPTIONS]

	OPTIONS:
	  -h,--help          Print this help text.
	  -q,--quiet         Do not print the brightness of the backlight.
	  -a,--adjust pct    Adjust backlight brightness by the given percentage.
	  -s,--set pct       Set the backlight brightness to the given percentage.

	This utility will read a backlight's brightness from

	  /sys/class/backlight/intel_backlight

	and display it to the user as a percentage of the backlight's maximum
	brightness. Additionally, this utility can also adjust the backlight's value
	before reading it for the user (by setting it to an explicit value or by
	adjusting it by a given percentage).


Reporting Bugs, Sending Patches and Giving Feedback
---------------------------------------------------

Contact the author e.g. via email.


Authors
-------

[Hemmo Nieminen](mailto:hemmo.nieminen@iki.fi)


Licensing
---------

See the file called LICENSE.
