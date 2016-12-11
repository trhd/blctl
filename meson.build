# blctl -- A small utility for controlling a backlight's brightness.
# Copyright (C) 2016 Hemmo Nieminen
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

project(
	'blctl',
	'c',
	version : '1.0.10',
	license : 'GPLv2',
	default_options :
	[
		'werror=true',
		'warning_level=3',
		'stdsplit=false',
		'c_std=c11'
	]
)


OPTARGS = subproject('optargs')
OPTARGS_HDR = OPTARGS.get_variable('OPTARGS_HDR')
OPTARGS_SRC = OPTARGS.get_variable('OPTARGS_SRC')

defines = [
	'-DSYSDIR_PATH="' + get_option('SYSDIR_PATH') + '"',
	'-DBLCTL_VERSION="' + meson.project_version() + '"'
]
if get_option('buildtype') == 'release'
	defines += [ '-DNDEBUG' ]
endif

BLCTL = executable(
	'blctl',
	[
		'blctl.c',
		OPTARGS_SRC
	],
	include_directories : OPTARGS_HDR,
	c_args : defines,
	install : true
)

subdir('tst')
