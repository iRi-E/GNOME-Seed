/* -*- mode: C; indent-tabs-mode: t; tab-width: 8; c-basic-offset: 2; -*- */

/*
 * This file is part of Seed, the GObject Introspection<->Javascript bindings.
 *
 * Seed is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 * Seed is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with Seed.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) Robert Carr 2009 <carrr@rpi.edu>
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <glib.h>
#include <glib-object.h>
#include "../libseed/seed.h"
#include "../libseed/seed-debug.h"
#include <girepository.h>
#include "config.h"

#define DEFAULT_PATH "."

SeedEngine *eng;
gboolean seed_interpreter_arg_print_version;
gchar *seed_interpreter_arg_exec_string;

gboolean seed_interpreter_parse_args (int *argc, char ***argv);

static void
seed_repl ()
{
  SeedScript *script;
  SeedException e;

  script = seed_make_script (eng->context, "repl = imports.repl", NULL, 0);

  if ((e = seed_script_exception (script)))
    {
      g_critical ("%s", seed_exception_to_string (eng->context, e));
      exit (EXIT_FAILURE);
    }

  seed_evaluate (eng->context, script, 0);

  if ((e = seed_script_exception (script)))
    {
      g_critical ("%s", seed_exception_to_string (eng->context, e));
      exit (EXIT_FAILURE);
    }

  g_free (script);
}

static void
seed_exec (gchar * filename)
{
  SeedObject global;
  SeedScript *script;
  SeedException e;
  FILE *file;
  GString *buffer;
  gchar c;

  /* If filename is NULL, read from stdin. */
  if (filename)
    {
      file = fopen (filename, "r");
      if (!file)
	{
	  g_critical ("File %s not found!", filename);
	  exit (EXIT_FAILURE);
	}
    }
  else
    file = stdin;

  /* Ignore the first line if it begins with '#!'. */
  c = fgetc (file);
  if (c == '#')
    {
      c = fgetc (file);
      if (c == '!')
	{
	  while ((c = fgetc (file)) != EOF)
	    {
	      if (c == '\n' || c == '\r')
		break;
            }
	}
      else
	{
	  ungetc (c, file);
	  c = '#';
	}
    }
  ungetc (c, file);

  buffer = g_string_new (NULL);

  while ((c = fgetc (file)) != EOF)
    g_string_append_c (buffer, c);

  if (file != stdin)
      fclose (file);

  script = seed_make_script (eng->context, buffer->str, filename, 1);
  if ((e = seed_script_exception (script)))
    {
      g_critical ("%s", seed_exception_to_string (eng->context, e));
      exit (EXIT_FAILURE);
    }

  g_string_free (buffer, TRUE);

  if (file != stdin)
    {
      /*
       * Make the file passed to the seed binary available as
       * imports.filename off the bat to prevent recursive running.
       */
      global = seed_context_get_global_object (eng->context);
      seed_importer_add_global (global, filename);
    }

  seed_evaluate (eng->context, script, 0);
  if ((e = seed_script_exception (script)))
    {
      g_critical ("%s", seed_exception_to_string (eng->context, e));
      exit (EXIT_FAILURE);
    }

  g_free (script);
}

static void
seed_exec_print_string (gchar * source)
{
  SeedException e = NULL;
  SeedValue val;
  gchar *val_str;

  val = seed_simple_evaluate (eng->context, source, &e);
  if (e)
    {
      g_critical ("%s", seed_exception_to_string (eng->context, e));
      exit (EXIT_FAILURE);
    }

  if (!seed_value_is_undefined (eng->context, val))
    {
      val_str = seed_value_to_string (eng->context, val, &e);
      if (e)
	{
	  g_critical ("%s", seed_exception_to_string (eng->context, e));
	  exit (EXIT_FAILURE);
	}

      g_print ("%s\n", val_str);
      g_free (val_str);
    }
}

gint
main (gint argc, gchar ** argv)
{
  g_set_prgname ("seed");
  g_thread_init (NULL);

  seed_interpreter_parse_args (&argc, &argv);

  if (seed_interpreter_arg_print_version)
    {
      /* --version option */
      g_print ("%s\n", "Seed " VERSION);
      exit (EXIT_SUCCESS);
    }

  eng = seed_init (&argc, &argv);

  seed_engine_set_search_path (eng, DEFAULT_PATH);

  if (seed_interpreter_arg_exec_string)
    {
      /* --execute option */
      seed_exec_print_string (seed_interpreter_arg_exec_string);
      g_free (seed_interpreter_arg_exec_string);
    }
  else if (argc > 1)
    seed_exec (argv[1]);            /* script file */
  else if (isatty (STDIN_FILENO))
    seed_repl ();                   /* stdin (interactive) */
  else
    seed_exec (NULL);               /* stdin (noninteractive) */

  return EXIT_SUCCESS;
}
