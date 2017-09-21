# blctl -- A small utility for controlling a backlight's brightness.
# Copyright (C) 2016-2017 Hemmo Nieminen
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

project(
	'blctl',
	'c',
	version : '2.0.0',
	license : 'GPLv3',
	default_options : [
		'werror=true',
		'warning_level=3',
		'stdsplit=false',
		'c_std=c11'
	]
)

defines = [
	'-DSYSDIR_PATH="' + get_option('SYSDIR_PATH') + '"',
	'-DBLCTL_VERSION="' + meson.project_version() + '"'
]

if get_option('buildtype') == 'release'
	defines += [ '-DNDEBUG' ]
endif

optargs = dependency('optargs', fallback : [ 'optargs', 'dep' ], static : true)

blctl = executable(
	'blctl',
	'blctl.c',
	dependencies : optargs,
	c_args : defines,
	install : true
)

subdir('tst')
