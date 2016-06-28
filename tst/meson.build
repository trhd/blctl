SYSDIR_SRC = meson.current_source_dir() + '/intel_backlight'
SYSDIR_DST = meson.current_build_dir() + '/intel_backlight'
TESTER = find_program('tester.sh')
find_program('sh')
VALGRIND_ARGS = [
	'--trace-children=yes',
	'--trace-children-skip=cp,rm',
	'--leak-check=full',
	'--show-leak-kinds=all',
	'--error-exitcode=1',
	'--suppressions=' + meson.current_source_dir() + '/valgrind.supp',
]
BLCTL_TEST_STATIC = executable(
	'blctl_tester',
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
	args : '--help',
	valgrind_args : VALGRIND_ARGS,
)

test(
	'prod: --foobar',
	BLCTL,
	args : '--foobar',
	valgrind_args : VALGRIND_ARGS,
	should_fail : true
)

test(
	'prod: --help + --quiet',
	BLCTL,
	args : [ '--quiet', '--help' ],
	valgrind_args : VALGRIND_ARGS,
)

test(
	'tester:',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '56.0', meson.current_build_dir() + '/blctl_tester'
	],
	valgrind_args : VALGRIND_ARGS,
	is_parallel : false
)

test(
	'tester: adjust: +10%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '66.0', meson.current_build_dir() + '/blctl_tester', '--adjust', '10%'
	],
	valgrind_args : VALGRIND_ARGS,
	is_parallel : false
)

test(
	'tester: adjust: -10%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '46.0', meson.current_build_dir() + '/blctl_tester', '--adjust', '-10%'
	],
	valgrind_args : VALGRIND_ARGS,
	is_parallel : false
)

test(
	'tester: adjust --quiet: -10%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', meson.current_build_dir() + '/blctl_tester', '--adjust', '-10%', '--quiet'
	],
	valgrind_args : VALGRIND_ARGS,
	is_parallel : false
)

test(
	'tester: adjust: 100%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '100.0', meson.current_build_dir() + '/blctl_tester', '--adjust', '100%'
	],
	valgrind_args : VALGRIND_ARGS,
	is_parallel : false
)

test(
	'tester: adjust --quiet: 100%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', meson.current_build_dir() + '/blctl_tester', '--adjust', '100%', '--quiet'
	],
	valgrind_args : VALGRIND_ARGS,
	is_parallel : false
)

test(
	'tester: adjust: -100%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '0.0', meson.current_build_dir() + '/blctl_tester', '--adjust', '-100%'
	],
	valgrind_args : VALGRIND_ARGS,
	is_parallel : false
)

test(
	'tester: adjust --quiet: -100%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', meson.current_build_dir() + '/blctl_tester', '--adjust', '-100%', '--quiet'
	],
	valgrind_args : VALGRIND_ARGS,
	is_parallel : false
)

test(
	'tester: set: 27%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '27.0', meson.current_build_dir() + '/blctl_tester', '--set', '27%'
	],
	valgrind_args : VALGRIND_ARGS,
	is_parallel : false
)

test(
	'tester: set --quiet: 27%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', meson.current_build_dir() + '/blctl_tester', '--set', '27%', '--quiet'
	],
	valgrind_args : VALGRIND_ARGS,
	is_parallel : false
)

test(
	'tester: set: 0',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '0.0', meson.current_build_dir() + '/blctl_tester', '--set', '0'
	],
	valgrind_args : VALGRIND_ARGS,
	is_parallel : false
)

test(
	'tester: set --quiet: 0',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', meson.current_build_dir() + '/blctl_tester', '--set', '0', '--quiet'
	],
	valgrind_args : VALGRIND_ARGS,
	is_parallel : false
)

test(
	'tester: set: 100',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '100.0', meson.current_build_dir() + '/blctl_tester', '--set', '100'
	],
	valgrind_args : VALGRIND_ARGS,
	is_parallel : false
)

test(
	'tester: set --quiet: 100',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', meson.current_build_dir() + '/blctl_tester', '--set', '100', '--quiet'
	],
	valgrind_args : VALGRIND_ARGS,
	is_parallel : false
)

test(
	'tester: set: -27%',
	TESTER,
	args :
	[
		SYSDIR_SRC, SYSDIR_DST, '', meson.current_build_dir() + '/blctl_tester', '--set', '-27%'
	],
	is_parallel : false,
	valgrind_args : VALGRIND_ARGS,
	should_fail : true
)
