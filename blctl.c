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
read_d_from_file(const char *f, int *d)
{
	assert(f);
	assert(d);

	char p[PATH_MAX + 1];
	FILE *h;
	int rv = 0;
	
	strcpy(p, sysdir);
	strcat(p, "/");
	strcat(p, f);

	h = fopen(p, "r");

	if (!h)
	{
		fprintf(stderr, "ERROR: Failed to open file '%s': %s.\n",
				p, strerror(errno));
		rv = -1;
	}
	else
	{
		if (fscanf(h, "%d", d) < 1)
		{
			fprintf(stderr, "ERROR: Failed to read an integer from '%s':"
					" %s.\n", p, strerror(ferror(h)));
			rv = -1;
		}

		if (fclose(h))
		{
			fprintf(stderr, "ERROR: Failed to close the file '%s': %s.\n",
					p, strerror(errno));
			rv = -1;
		}
	}

	return rv;
}

/************************************************************************/

static int
write_d_to_file(const char *f, int c)
{
	assert(f);

	char p[PATH_MAX + 1];
	FILE *h;
	int rv = 0;
	
	strcpy(p, sysdir);
	strcat(p, "/");
	strcat(p, f);

	h = fopen(p, "w");

	if (!h)
	{
		fprintf(stderr, "ERROR: Failed to open file '%s': %s.\n",
				p, strerror(errno));
		rv = -1;
	}
	else
	{
		if (fprintf(h, "%d", c) < 1)
		{
			fprintf(stderr, "ERROR: Failed to write an integer to file "
					"'%s': %s.\n", p, strerror(ferror(h)));
			rv = -1;
		}

		if (fclose(h))
		{
			fprintf(stderr, "ERROR: Failed to close the file '%s': %s.\n",
					p, strerror(errno));
			rv = -1;
		}
	}

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
	
	if (get_maximum_value(&m))
		return -1;

	c = c * m / 100;

	if (c < 0)
	{
		fprintf(stderr, "ERROR: Cannot set backlight brighness to a "
				"negative percentage.\n");
		return -1;
	}

	return write_d_to_file("brightness", c);
}
/************************************************************************/

static int
percentage_to_float(const char *p, float *f)
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

	if (get_percentage(&c) || percentage_to_float(pct, &a))
		return -1;

	c = c + a < 0 ? 0 : c + a;

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

	if (percentage_to_float(p, &f))
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
			.description = "Do not print the brighness of the backlight.",
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

	if (optargs_opt_by_long(opts, "help"))
	{
		optargs_print_help(av[0], about, opts, NULL);
		return EXIT_SUCCESS;
	}

	adj = optargs_opt_by_long(opts, "adjust");
	set = optargs_opt_by_long(opts, "set");

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
