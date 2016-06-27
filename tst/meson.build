SYSDIR_SRC = meson.current_source_dir() + '/intel_backlight'
SYSDIR_DST = meson.current_build_dir() + '/intel_backlight'
TESTER = find_program('tester.sh')
BLCTL_TEST_STATIC = executable(
	'blctl_test_static',
	[
		'../blctl.c',
		OPTARGS_SRC
	],
	include_directories : OPTARGS_HDR,
	c_args :
	[
		'-DSYSDIR_PATH="' + SYSDIR_DST + '"'
	]
)

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
	'static:',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '56.0', meson.current_build_dir() + '/blctl_test_static'
	],
	is_parallel : false
)

test(
	'static: adjust: +10%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '66.0', meson.current_build_dir() + '/blctl_test_static', '--adjust', '10%'
	],
	is_parallel : false
)

test(
	'static: adjust: -10%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '46.0', meson.current_build_dir() + '/blctl_test_static', '--adjust', '-10%'
	],
	is_parallel : false
)

test(
	'static: adjust --quiet: -10%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', meson.current_build_dir() + '/blctl_test_static', '--adjust', '-10%', '--quiet'
	],
	is_parallel : false
)

test(
	'static: set: 27%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '27.0', meson.current_build_dir() + '/blctl_test_static', '--set', '27%'
	],
	is_parallel : false
)

test(
	'static: set --quiet: 27%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', meson.current_build_dir() + '/blctl_test_static', '--set', '27%', '--quiet'
	],
	is_parallel : false
)

test(
	'static: set: -27%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', meson.current_build_dir() + '/blctl_test_static', '--set', '-27%'
	],
	is_parallel : false,
	should_fail : true
)
