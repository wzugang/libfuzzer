/*
  Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005,
  2006, 2007, 2008, 2009, 2010, 2011 Free Software Foundation, Inc.

  This file is part of GNU Inetutils.

  GNU Inetutils is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or (at
  your option) any later version.

  GNU Inetutils is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see `http://www.gnu.org/licenses/'. */

#include <config.h>

#if HAVE_UTMPX_H && defined SOLARIS
# include <utmpx.h>	/* updwtmp() */
#endif

#define KEEP_OPEN
#include "logwtmp.c"

/* Solaris does not provide logwtmp(3c).
 * It is needed in addition to logwtmp_keep_open().
 */
#if HAVE_UPDWTMP && defined SOLARIS
void
logwtmp (char *line, char *name, char *host)
{
  struct utmp ut;
#ifdef HAVE_STRUCT_UTMP_UT_TV
  struct timeval tv;
#endif

  memset (&ut, 0, sizeof (ut));
#ifdef HAVE_STRUCT_UTMP_UT_TYPE
  ut.ut_type = USER_PROCESS;
#endif
  strncpy (ut.ut_line, line, sizeof ut.ut_line);
  strncpy (ut.ut_name, name, sizeof ut.ut_name);
#ifdef HAVE_STRUCT_UTMP_UT_HOST
  strncpy (ut.ut_host, host, sizeof ut.ut_host);
#endif

# ifdef HAVE_STRUCT_UTMP_UT_TV
  gettimeofday (&tv, NULL);
  ut.ut_tv.tv_sec = tv.tv_sec;
  ut.ut_tv.tv_usec = tv.tv_usec;
# else
  time (&ut.ut_time);
# endif

  updwtmp (PATH_WTMP, &ut);
}
#endif /* HAVE_UPDWTMP && defined SOLARIS */