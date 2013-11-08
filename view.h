/******************************************************************************
 Copyright 2013 Victor Yurkovsky

    This file is part of PCBFeet project

    PCBFeet is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PCBFeet is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PCBFeet.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#pragma once
#include <cairo.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <malloc.h>

#include "types.h"
#include "parm.h"
/* View-related information is contained here...
 * The view is measured in pixels.  The most important variable is
 * scale - how many centimils of the document fit into a pixel.
 * 
*/

typedef enum eMode {
  MODE_IDLE,
  MODE_SET_ORIGIN
} eMode;


typedef struct sGrid {
  sPoint origin;
  coord unit;       //in centimils
  sNearestPoint nearest;
  // TODO: these probably should be transient...
  sPoint pxOrigin;  //after drawing, upper left pixel coordinates
  sPoint xyCnt;     //after drawing, x and y gridpoint count
} sGrid;

struct sDocument;
typedef struct sView {
  
  struct sDocument* document;
  // all coordinates here are in the doc coordinate system
  sPoint origin;
  int scale;  //this many centimils to a pixel

  sGrid grid;
  
  //screen pixel measurements
  int width;
  int height;
  sPoint pxMouse;

//  eMode mode;
  gboolean mode_set_origin;
  
  //important widgets
  GtkWidget* frame;
  GtkWidget* canvas;
  GtkWidget* hruler;
  GtkWidget* vruler;
  GtkLabel*  status_xy;  //display coordinates
  GtkWidget* but_origin;
  GtkWidget* objects;    //vbox with obj descriptions
  // array of parameter widgets
  GtkWidget* parm_widget[32]; //TODO: check the size!
} sView;

sView* view_new();
void   view_initialize(sView* view,const char* uiname);

