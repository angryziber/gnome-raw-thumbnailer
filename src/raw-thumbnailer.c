/**
   raw-thumbnailer.c
   Copyright (c) 2011 Hub Figuière
   Copyright (c) 2007 Novell, Inc.
   Copyright (C) 2003,2004 Bastien Nocera <hadess@hadess.net>
    
   Author: Hub Figuière <hub@figuiere.net>
   Derived from the totem video thumbnailer.
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
	 02110-1301  USA
*/

#include "config.h"

#include <stdio.h>

#include <gio/gio.h>
#include <glib/gi18n.h>
#include <libopenraw-gnome/gdkpixbuf.h>

static gboolean jpeg_output = FALSE;
static gboolean output_size = 128;
static gboolean verbose = FALSE;
static gboolean g_fatal_warnings = FALSE;
static char **filenames = NULL;

static void
save_pixbuf (GdkPixbuf *pixbuf, const char *fname, const char *path, int size)
{
  GdkPixbuf *small = NULL;
  GError *err = NULL;
  char *a_width, *a_height;
  int width, height;
  height = gdk_pixbuf_get_height (pixbuf);
  width = gdk_pixbuf_get_width (pixbuf);
  
  gboolean is_3x2_aspect = g_str_has_suffix(fname, ".cr2") || g_str_has_suffix(fname, ".CR2") ||
                           g_str_has_suffix(fname, ".nef") || g_str_has_suffix(fname, ".NEF");

  if (is_3x2_aspect && width == 160 && height == 120) {
    // we've got horizontal embedded exif thumbnail with black bars
    small = gdk_pixbuf_new (GDK_COLORSPACE_RGB, FALSE, 8, 160, 104);
    gdk_pixbuf_copy_area (pixbuf, 0, 8, 160, 104, small, 0, 0);
  }
  else
  if (is_3x2_aspect && width == 120 && height == 160) {
    // we've got vertical embedded exif thumbnail with black bars
    small = gdk_pixbuf_new (GDK_COLORSPACE_RGB, FALSE, 8, 104, 160);
    gdk_pixbuf_copy_area (pixbuf, 8, 0, 104, 160, small, 0, 0);
  }
  else
  if (size < height || size < width) {
    int d_width, d_height;
    
    if (width > height) {
      d_width = size;
      d_height = size * height / width;
    } 
    else {
      d_height = size;
      d_width = size * width / height;
    }
    
    small = gdk_pixbuf_scale_simple (pixbuf, d_width, d_height,
				     GDK_INTERP_TILES);
  }
  else {
    small = pixbuf;
    g_object_ref (small);
  }
  
  a_width = g_strdup_printf ("%d", width);
  a_height = g_strdup_printf ("%d", height);
  
  if (gdk_pixbuf_save (small, path, 
		       jpeg_output ? "jpeg" : "png", &err,
		       "tEXt::Thumb::Image::Width", a_width,
		       "tEXt::Thumb::Image::Height", a_height,
		       NULL) == FALSE) {
    if (err != NULL) {
      g_print ("raw-thumbnailer couldn't write the thumbnail '%s': %s\n", path, err->message);
      g_error_free (err);
    } 
    else {
      g_print ("raw-thumbnailer couldn't write the thumbnail '%s'\n", path);
    }
  }
  
  g_free(a_width);
  g_free(a_height);
  g_object_unref(small);
}

static const GOptionEntry entries[] = {
  { "jpeg", 'j',  0, G_OPTION_ARG_NONE, &jpeg_output, "Output the thumbnail as a JPEG instead of PNG", NULL },
  { "size", 's', 0, G_OPTION_ARG_INT, &output_size, "Size of the thumbnail in pixels", NULL },
  { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Output debug information", NULL },
#ifndef THUMB_DEBUG
  {"g-fatal-warnings", 0, 0, G_OPTION_ARG_NONE, &g_fatal_warnings, "Make all warnings fatal", NULL},
#endif /* THUMB_DEBUG */
  { G_OPTION_REMAINING, '\0', 0, G_OPTION_ARG_FILENAME_ARRAY, &filenames, NULL, "[FILE...]" },
  { NULL }
};

int main (int argc, char ** argv)
{
  const char *output_name;
  const char *input_name;
  GOptionContext *context;
  GError *err = NULL;
  gboolean success;
  int size;
  GdkPixbuf *pixbuf;
  
  context = g_option_context_new ("Thumbnail camera RAW files.");
  g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);
  
  if (g_option_context_parse (context, &argc, &argv, &err) == FALSE) {
    g_print ("couldn't parse command-line options: %s\n", err->message);
    g_error_free (err);
    return 1;
  }
  if (g_fatal_warnings) {
    GLogLevelFlags fatal_mask;
    
    fatal_mask = g_log_set_always_fatal (G_LOG_FATAL_MASK);
    fatal_mask |= G_LOG_LEVEL_WARNING | G_LOG_LEVEL_CRITICAL;
    g_log_set_always_fatal (fatal_mask);
  }
  if (filenames == NULL || g_strv_length (filenames) != 2) {
    g_print ("Input and output files are expected. Use -h for help.\n");
    return 1;
  }
  
  input_name = filenames[0];
  output_name = filenames[1];
  
  GFile* file = g_file_new_for_commandline_arg(input_name);
  char* uri = g_file_get_uri(file);
  g_object_unref(file);
  
  char* inputfname = g_filename_from_uri (uri, NULL, NULL);
  g_free(uri);

  int requested_size = output_size > 160 && output_size < 362 ? 512 : output_size;
  pixbuf = or_gdkpixbuf_extract_rotated_thumbnail(inputfname, requested_size);
  
  save_pixbuf(pixbuf, inputfname, output_name, output_size);
  
  g_free(inputfname);
  g_object_unref(pixbuf);	
  g_option_context_free(context);
  
  return 0;
}

