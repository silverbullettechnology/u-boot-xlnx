/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <exports.h>

int print_something ()
{
	int i;


	printf ("This is the second function\n");

	printf ("\n\n");
	return (0);
}
