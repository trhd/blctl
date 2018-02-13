/**
 * blctl -- A small utility for controlling a backlight's brightness.
 * Copyright (C) 2016-2018 Hemmo Nieminen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <assert.h>
#include <errno.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "optargs.h"

static const char *about = "This utility will read a backlight's "
	"brightness from\n\n  " SYSDIR_PATH "\n\nand display it to the user "
	"as a percentage of the backlight's maximum brightness. Additionally, "
	"this utility can also adjust the backlight's brightness before "
	"displaying it to the user (by setting it to an explicit percentage "
	"or by adjusting it by the given amount).";

enum action
{
	HELP,
	VERSION,
	QUIET,
	ADJUST,
	SET,
	_ACTION_COUNT
};

/************************************************************************/

static void
print_version_information()
{
	printf("blctl v" BLCTL_VERSION ", GPLv3, Copyright (C) 2016-2017 Hemmo Nieminen\n");
}

/************************************************************************/

static bool
mksysfspath(const char *f, char *b, size_t bl)
{
	assert(f);
	assert(b);
	assert(bl);

	if (bl <= strlen(SYSDIR_PATH) + 1 + strlen(f))
	{
		fprintf(stderr, "ERROR: Failed to form the path to the sysfs "
				"control file: %s.\n", strerror(ENOMEM));
		return true;
	}

	sprintf(b, "%s/%s", SYSDIR_PATH, f);

	return false;
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

static bool
close_file_handle(const char *f, FILE *h)
{
	assert(f);
	assert(h);

	if (fclose(h))
	{
		fprintf(stderr, "ERROR: Failed to close the file handle for '%s': %s.\n",
				f, strerror(errno));
		return true;
	}

	return false;
}

/************************************************************************/

static bool
scan_d_from_file(const char *f, FILE *h, int *d)
{
	assert(f);
	assert(h);
	assert(d);

	if (fscanf(h, "%d", d) < 1)
	{
		fprintf(stderr, "ERROR: Failed to read an integer from '%s':"
				" %s.\n", f, strerror(ferror(h)));
		return true;
	}

	return false;
}

/************************************************************************/

static bool
print_d_to_file(const char *f, FILE *h, int d)
{
	assert(f);
	assert(h);

	if (fprintf(h, "%d", d) < 1)
	{
		fprintf(stderr, "ERROR: Failed to write an integer to file "
				"'%s': %s.\n", f, strerror(ferror(h)));
		return true;
	}

	return false;
}

/************************************************************************/

static bool
read_d_from_file(const char *f, int *d)
{
	assert(f);
	assert(d);

	char p[PATH_MAX + 1];
	FILE *h = NULL;
	bool rv;

	if (mksysfspath(f, p, sizeof(p))
			|| !(h = open_file(p, "r"))
			|| scan_d_from_file(p, h, d))
		rv = true;
	else
		rv = false;

	return (h && close_file_handle(f, h)) || rv;
}

/************************************************************************/

static bool
write_d_to_file(const char *f, int c)
{
	assert(f);

	char p[PATH_MAX + 1];
	FILE *h = NULL;
	bool rv;

	if (mksysfspath(f, p, sizeof(p))
			|| !(h = open_file(p, "w"))
			|| print_d_to_file(p, h, c))
		rv = true;
	else
		rv = false;

	return (h && close_file_handle(f, h)) || rv;
}

/************************************************************************/

static bool
get_current_value(int *d)
{
	assert(d);

	return read_d_from_file("brightness", d) || *d < 0;
}

/************************************************************************/

static bool
get_maximum_value(int *d)
{
	assert(d);

	return read_d_from_file("max_brightness", d) || *d <= 0;
}

/************************************************************************/

static int
round_float(float f)
{
	int r = f;

	if (f - r >= 0.5)
		return r + 1;
	else
		return r;
}

/************************************************************************/

static bool
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
		c = c * m / 100.0;
		return write_d_to_file("brightness", round_float(c));
	}

	return true;
}
/************************************************************************/

static bool
str_to_float(const char *p, float *f)
{
	assert(p);
	assert(f);

	if (sscanf(p, "%f", f) != 1)
	{
		fprintf(stderr, "ERROR: Failed to parse input '%s' (expected "
				"percentage).\n", p);
		return true;
	}

	return false;
}

/************************************************************************/

static bool
get_percentage(float *f)
{
	assert(f);

	int c, m;

	if (get_current_value(&c) || get_maximum_value(&m))
		return true;

	*f = 100.0 * c / m;

	return false;
}

/************************************************************************/

static bool
adjust_current_percentage_by(const char *pct)
{
	assert(pct);

	float c, a;

	if (get_percentage(&c) || str_to_float(pct, &a))
		return true;

	c = c + a;

	if (c < 0)
		c = 0;
	else if (c > 100)
		c = 100;

	return set_current_percentage(c);
}

/************************************************************************/

static bool
print_percentage()
{
	float p;

	if (get_percentage(&p))
		return true;

	printf("%.1f\n", p);

	return false;
}

/************************************************************************/

static bool
set_percentage_to(const char *p)
{
	assert(p);

	float f;

	if (str_to_float(p, &f))
		return true;

	return set_current_percentage(f);
}

/************************************************************************/

int
main(int ac, char **av)
{
	struct optargs_option opts[] =
	{
		[ADJUST] =
		{
			.description = "Adjust backlight brightness by the given "
				"percentage.",
			.long_option = "adjust",
			.short_option = 'a',
			.argument = (struct optargs_argument [])
			{
				{
					.name = "pct",
					.type = optargs_argument_any
				},
				optargs_argument_eol
			}
		},

		[SET] =
		{
			.description = "Set the backlight brightness to the given "
				"percentage.",
			.long_option = "set",
			.short_option = 's',
			.argument = (struct optargs_argument [])
			{
				{
					.name = "pct",
					.type = optargs_argument_any
				},
				optargs_argument_eol
			}
		},

		[HELP] =
		{
			.description = "Print this help text.",
			.long_option = "help",
			.short_option = 'h',
		},

		[QUIET] =
		{
			.description = "Do not print the brightness of the backlight.",
			.long_option = "quiet",
			.short_option = 'q',
		},

		[VERSION] =
		{
			.description = "Print version information.",
			.long_option = "version",
			.short_option = 'v'
		},

		[_ACTION_COUNT] = optargs_option_eol
	};
	const char *set, *adj;

	if (optargs_parse_options(ac, (const char **)av, opts) < 0)
	{
		optargs_print_help(av[0], about, opts, NULL);
		return EXIT_FAILURE;
	}

	if (optargs_option_count(opts, HELP))
	{
		optargs_print_help(av[0], about, opts, NULL);
		return EXIT_SUCCESS;
	}

	if (optargs_option_count(opts, VERSION))
	{
		print_version_information();
		return EXIT_SUCCESS;
	}

	adj = optargs_option_string(opts, ADJUST);
	set = optargs_option_string(opts, SET);

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

	if (optargs_option_count(opts, QUIET))
		return EXIT_SUCCESS;

	return print_percentage() ? EXIT_FAILURE : EXIT_SUCCESS;
}

/************************************************************************/
