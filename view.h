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
/* View-related information is contained here...
 * The view is measured in pixels.  The most important variable is
 * scale - how many centimils of the document fit into a pixel.
 * 
*/
typedef int coord;

typedef enum eMode {
  mode_idle,
  mode_set_origin
} eMode;

typedef struct sPoint{
  coord x;
  coord y;
} sPoint;

typedef struct sView {
  // all coordinates here are in the view coordinate system
  sPoint origin;
  sPoint grid_origin;
  sPoint mouse;
  coord grid_unit; //in centimils
  
  float scale;  //this many centimils to a pixel

  
  //screen pixel measurements
  int width;
  int height;

  eMode mode;
  
  //important widgets
  GtkWidget* frame;
  GtkWidget* canvas;
  GtkWidget* hruler;
  GtkWidget* vruler;
  GtkLabel*  status_xy;  //display coordinates
  GtkWidget* but_origin;
  
  
} sView;

sView* view_new();
void   view_initialize(sView* view,const char* uiname);

