/**
 * blctl -- A small utility for controlling backlight's brightness.
 * Copyright (C) 2016 Hemmo Nieminen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <assert.h>
#include <errno.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "optargs.h"

static const char *sysdir = SYSDIR_PATH;
static const char *about = "This utility will read a backlight's "
	"brightness from\n\n  " SYSDIR_PATH "\n\nand display it to the user "
	"as a percentage of the backlight's maximum brightness. Additionally,"
	" this utility can also adjust the backlight's value before reading "
	"it for the user (by setting it to an explicit value or by adjusting "
	"it by a given percentage).";

enum action
{
	HELP,
	QUIET,
	ADJUST,
	SET,
	_ACTION_COUNT
};

/************************************************************************/

static int
mksysfspath(const char *f, char *b, size_t bl)
{
	assert(f);
	assert(b);

	strncpy(b, sysdir, bl);
	bl -= strlen(sysdir);

	strncat(b, "/", bl);
	bl -= 1;

	strncat(b, f, bl);
	bl -= strlen(f);

	if (bl <= 0)
	{
		fprintf(stderr, "ERROR: Failed to form the path to the sysfs "
				"control file: %s.\n", strerror(errno));
		return -1;
	}

	return 0;
}

/************************************************************************/

static FILE *
open_file(const char *f, const char *m)
{
	assert(f);
	assert(m);

	FILE *rv = fopen(f, m);

	if (!rv)
		fprintf(stderr, "ERROR: Failed to open file '%s': %s.\n", f, strerror(errno));

	return rv;
}

/************************************************************************/

static int
close_file_handle(const char *f, FILE *h)
{
	assert(f);
	assert(h);

	if (fclose(h))
	{
		fprintf(stderr, "ERROR: Failed to close the file handle for '%s': %s.\n",
				f, strerror(errno));
		return -1;
	}

	return 0;
}

/************************************************************************/

static int
scan_d_from_file(const char *f, FILE *h, int *d)
{
	assert(f);
	assert(h);
	assert(d);

	if (fscanf(h, "%d", d) < 1)
	{
		fprintf(stderr, "ERROR: Failed to read an integer from '%s':"
				" %s.\n", f, strerror(ferror(h)));
		return -1;
	}

	return 0;
}

/************************************************************************/

static int
print_d_to_file(const char *f, FILE *h, int d)
{
	assert(f);
	assert(h);

	if (fprintf(h, "%d", d) < 1)
	{
		fprintf(stderr, "ERROR: Failed to write an integer to file "
				"'%s': %s.\n", f, strerror(ferror(h)));
		return -1;
	}

	return 0;
}

/************************************************************************/

static int
read_d_from_file(const char *f, int *d)
{
	assert(f);
	assert(d);

	char p[PATH_MAX + 1];
	FILE *h = NULL;
	int rv;

	if (mksysfspath(f, p, sizeof(p))
			|| !(h = open_file(p, "r"))
			|| scan_d_from_file(p, h, d))
		rv = -1;
	else
		rv = 0;

	if (h && close_file_handle(f, h))
		rv = -1;

	return rv;
}

/************************************************************************/

static int
write_d_to_file(const char *f, int c)
{
	assert(f);

	char p[PATH_MAX + 1];
	FILE *h = NULL;
	int rv;

	if (mksysfspath(f, p, sizeof(p))
			|| !(h = open_file(p, "w"))
			|| print_d_to_file(p, h, c))
		rv = -1;
	else
		rv = 0;

	if (h && close_file_handle(f, h))
		rv = -1;

	return rv;
}

/************************************************************************/

static int
get_current_value(int *d)
{
	assert(d);

	return read_d_from_file("brightness", d)
		|| *d >= 0 ? 0 : -1;
}

/************************************************************************/

static int
get_maximum_value(int *d)
{
	assert(d);

	return read_d_from_file("max_brightness", d)
		|| *d > 0 ? 0 : -1;
}

/************************************************************************/

static int
set_current_percentage(float c)
{
	int m;

	if (c < 0)
		fprintf(stderr, "ERROR: Cannot set backlight brightness percentage"
				" to a negative value.\n");
	else if (c > 100)
		fprintf(stderr, "ERROR: Cannot set backlight brightness percentage"
				" to a value exceeding 100%%.\n");
	else if (get_maximum_value(&m))
	{}
	else
	{
		c = c * m / 100;
		return write_d_to_file("brightness", c);
	}

	return -1;
}
/************************************************************************/

static int
percentage_str_to_float(const char *p, float *f)
{
	assert(p);
	assert(f);

	if (sscanf(p, "%f", f) != 1)
	{
		fprintf(stderr, "ERROR: Failed to parse input '%s' (expected "
				"percentage).\n", p);
		return -1;
	}

	return 0;
}

/************************************************************************/

static int
get_percentage(float *f)
{
	assert(f);

	int c, m;

	if (get_current_value(&c) || get_maximum_value(&m))
		return -1;

	*f = 100 * c / m;

	return 0;
}

/************************************************************************/

static int
adjust_current_percentage_by(const char *pct)
{
	assert(pct);

	float c, a;

	if (get_percentage(&c) || percentage_str_to_float(pct, &a))
		return -1;

	c = c + a;

	if (c < 0)
		c = 0;
	else if (c > 100)
		c = 100;

	return set_current_percentage(c);
}

/************************************************************************/

static int
print_percentage()
{
	float p;

	if (get_percentage(&p))
		return -1;

	printf("%.1f\n", p);

	return 0;
}

/************************************************************************/

static int
set_percentage_to(const char *p)
{
	assert(p);

	float f;

	if (percentage_str_to_float(p, &f))
		return -1;

	return set_current_percentage(f);
}

/************************************************************************/

int
main(int ac, char **av)
{
	struct optargs_opt opts[] =
	{
		[ADJUST] =
		{
			.description = "Adjust backlight brightness by the given "
				"percentage.",
			.long_option = "adjust",
			.short_option = 'a',
			.argument =
			{
				.name = "pct",
				.mandatory = optargs_yes
			},
		},

		[SET] =
		{
			.description = "Set the backlight brightness to the given "
				"percentage.",
			.long_option = "set",
			.short_option = 's',
			.argument =
			{
				.name = "pct",
				.mandatory = optargs_yes
			},
		},

		[HELP] =
		{
			.description = "Print this help text.",
			.long_option = "help",
			.short_option = 'h',
			.argument = { .mandatory = optargs_no },
		},

		[QUIET] =
		{
			.description = "Do not print the brightness of the backlight.",
			.long_option = "quiet",
			.short_option = 'q',
			.argument = { .mandatory = optargs_no },
		},

		[_ACTION_COUNT] = optargs_opt_eol
	};
	const char *set, *adj;

	if (optargs_parse(ac, (const char **)av, opts))
	{
		optargs_print_help(av[0], about, opts, NULL);
		return EXIT_FAILURE;
	}

	if (optargs_opt_by_index(opts, HELP))
	{
		optargs_print_help(av[0], about, opts, NULL);
		return EXIT_SUCCESS;
	}

	adj = optargs_opt_by_index(opts, ADJUST);
	set = optargs_opt_by_index(opts, SET);

	if (set && adj)
	{
		fprintf(stderr, "ERROR: Conflicting options; only one of "
				"\"adjust\" and \"set\" can be given at a time.\n");
		return EXIT_FAILURE;
	}

	if (set && set_percentage_to(set))
		return EXIT_FAILURE;

	if (adj && adjust_current_percentage_by(adj))
		return EXIT_FAILURE;

	if (optargs_opt_by_long(opts, "quiet"))
		return EXIT_SUCCESS;

	return print_percentage() ? EXIT_FAILURE : EXIT_SUCCESS;
}

/************************************************************************/
