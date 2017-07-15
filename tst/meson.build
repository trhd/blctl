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

### Setting things up. ###################################################

SYSDIR_SRC = meson.current_source_dir() + '/intel_backlight'
SYSDIR_DST = meson.current_build_dir() + '/intel_backlight'
TESTER = find_program('tester.sh')
find_program('sh')
BLCTL_TEST_STATIC = executable(
	'blctl_tester',
	[
		meson.source_root() + '/blctl.c',
		OPTARGS_SRC
	],
	include_directories : OPTARGS_HDR,
	c_args :
	[
		'-DSYSDIR_PATH="' + SYSDIR_DST + '"',
		'-DBLCTL_VERSION="' + meson.project_version() + '"'
	]
)

add_test_setup(
	'valgrind',
	exe_wrapper : [
		'valgrind',
			'--trace-children=yes',
			'--trace-children-skip=bash,cp,rm',
			'--leak-check=full',
			'--show-leak-kinds=all',
			'--suppressions=' + meson.current_source_dir() + '/valgrind.supp',
			'--error-exitcode=1'],
	timeout_multiplier : 20
	)

### Some basic tests with the production binary. #########################

test(
	'prod: --help',
	BLCTL,
	args : '--help'
)

test(
	'prod: --foobar',
	BLCTL,
	args : '--foobar',
	should_fail : true
)

test(
	'prod: --help + --quiet',
	BLCTL,
	args : [ '--quiet', '--help' ]
)

test(
	'prod: --version',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST,
			'blctl v@0@, GPLv2, Copyright (C) 2016-2017 Hemmo Nieminen'.format(meson.project_version()),
			BLCTL.full_path(), '--version'
	]
)

test(
	'tester:',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '56.7', BLCTL_TEST_STATIC.full_path()
	],
	is_parallel : false
)

### Adjusting current value. #############################################

test(
	'tester: adjust: +10%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '66.7', BLCTL_TEST_STATIC.full_path(), '--adjust', '10%'
	],
	is_parallel : false
)

test(
	'tester: adjust: -10%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '46.7', BLCTL_TEST_STATIC.full_path(), '--adjust', '-10%'
	],
	is_parallel : false
)

test(
	'tester: adjust --quiet: -10%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', BLCTL_TEST_STATIC.full_path(), '--adjust', '-10%', '--quiet'
	],
	is_parallel : false
)

test(
	'tester: adjust: 100%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '100.0', BLCTL_TEST_STATIC.full_path(), '--adjust', '100%'
	],
	is_parallel : false
)

test(
	'tester: adjust --quiet: 100%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', BLCTL_TEST_STATIC.full_path(), '--adjust', '100%', '--quiet'
	],
	is_parallel : false
)

test(
	'tester: adjust: -100%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '0.0', BLCTL_TEST_STATIC.full_path(), '--adjust', '-100%'
	],
	is_parallel : false
)

test(
	'tester: adjust --quiet: -100%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', BLCTL_TEST_STATIC.full_path(), '--adjust', '-100%', '--quiet'
	],
	is_parallel : false
)

test(
	'tester: adjust: 150%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '100.0', BLCTL_TEST_STATIC.full_path(), '--adjust', '150%'
	],
	is_parallel : false
)

test(
	'tester: adjust: -150%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '0.0', BLCTL_TEST_STATIC.full_path(), '--adjust', '-150%'
	],
	is_parallel : false
)

### Setting explicit values. #############################################

test(
	'tester: set: 27%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '27.0', BLCTL_TEST_STATIC.full_path(), '--set', '27%'
	],
	is_parallel : false
)

test(
	'tester: set --quiet: 27%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', BLCTL_TEST_STATIC.full_path(), '--set', '27%', '--quiet'
	],
	is_parallel : false
)

test(
	'tester: set: 0',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '0.0', BLCTL_TEST_STATIC.full_path(), '--set', '0'
	],
	is_parallel : false
)

test(
	'tester: set --quiet: 0',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', BLCTL_TEST_STATIC.full_path(), '--set', '0', '--quiet'
	],
	is_parallel : false
)

test(
	'tester: set: 100',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '100.0', BLCTL_TEST_STATIC.full_path(), '--set', '100'
	],
	is_parallel : false
)

test(
	'tester: set --quiet: 100',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', BLCTL_TEST_STATIC.full_path(), '--set', '100', '--quiet'
	],
	is_parallel : false
)

test(
	'tester: set: 100.1%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', BLCTL_TEST_STATIC.full_path(), '--set', '100.1%'
	],
	is_parallel : false,
	should_fail : true
)

test(
	'tester: set: -0.1%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', BLCTL_TEST_STATIC.full_path(), '--set', '-0.1%'
	],
	is_parallel : false,
	should_fail : true
)

### The end. #############################################################
