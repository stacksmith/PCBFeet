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
  //parameter editor

typedef enum eParmWidget {
  e_label_name,  e_name,
  e_label_number,  e_number,
  //
  e_box_p1,  e_label_p1,  e_p1_x,  e_p1_y,
  e_box_p2,  e_label_p2,  e_p2_x,  e_p2_y,
  e_label_hole,  e_hole,
  //
  e_label_thick,  e_thick,
  e_label_clear,  e_clear,
  e_label_mask,  e_mask,
  //
  e_label_shape,  e_shape,
  e_label_layer,  e_layer,
  pw_last
} eParmWidget;

static char* parm_widget_name[]={
  "parm_label_name", "parm_name", "parm_label_number", "parm_number",
//
  "parm_box_p1", "parm_label_p1", "parm_p1_x", "parm_p1_y",
  "parm_box_p2", "parm_label_p2", "parm_p2_x", "parm_p2_y",
  "parm_label_hole", "parm_hole",
//
  "parm_label_thick","parm_thick",
  "parm_label_clear", "parm_clear",
  "parm_label_mask", "parm_mask",
//
  "parm_label_shape","parm_shape",
  "parm_label_layer", "parm_layer"
};
/* UIMASKS are bitmaps indicating which widgets are required for specific types of objs
 See multiple selection mechanism for how ANDing the masks leaves a bitmap of relevant
 widgets to activate for the selection
 
 */
#define UIMASK_PIN  (1<<e_label_name) + (1<<e_name) + (1<<e_label_number ) + (1<<e_number  ) \
 + (1<<e_box_p1) + (1<<e_label_p1) + (1<<e_p1_x) + (1<<e_p1_y ) \
 + (0<<e_box_p2) + (0<<e_label_p2) + (0<<e_p2_x) + (0<<e_p2_y ) \
 + (1<<e_label_hole) + (1<< e_hole ) \
 + (1<<e_label_thick) + (1<< e_thick ) \
 + (1<<e_label_clear) + (1<<e_clear ) \
 + (1<<e_label_mask ) + (1<<e_mask ) \
 + (1<<e_label_shape) + (1<<e_shape ) \
 + (1<<e_label_layer) + (1<<e_layer)

#define UIMASK_PAD  (1<<e_label_name) + (1<<e_name) + (1<<e_label_number ) + (1<<e_number  ) \
 + (1<<e_box_p1) + (1<<e_label_p1) + (1<<e_p1_x) + (1<<e_p1_y ) \
 + (1<<e_box_p2) + (1<<e_label_p2) + (0<<e_p2_x) + (0<<e_p2_y ) \
 + (0<<e_label_hole) + (0<< e_hole ) \
 + (1<<e_label_thick) + (1<< e_thick ) \
 + (1<<e_label_clear) + (1<<e_clear ) \
 + (1<<e_label_mask ) + (1<<e_mask ) \
 + (1<<e_label_shape) + (1<<e_shape ) \
 + (1<<e_label_layer) + (1<<e_layer)

#define UIMASK_LINE  (0<<e_label_name) + (0<<e_name) + (0<<e_label_number ) + (0<<e_number  ) \
 + (1<<e_box_p1) + (1<<e_label_p1) + (1<<e_p1_x) + (1<<e_p1_y ) \
 + (1<<e_box_p2) + (1<<e_label_p2) + (0<<e_p2_x) + (0<<e_p2_y ) \
 + (0<<e_label_hole) + (0<< e_hole ) \
 + (1<<e_label_thick) + (1<< e_thick ) \
 + (0<<e_label_clear) + (0<<e_clear ) \
 + (0<<e_label_mask ) + (0<<e_mask ) \
 + (0<<e_label_shape) + (0<<e_shape ) \
 + (1<<e_label_layer) + (1<<e_layer)

//This one is the starting point for multiple selection
#define UIMASK_MULTIPLE  (0<<e_label_name) + (0<<e_name) + (0<<e_label_number ) + (0<<e_number  ) \
 + (1<<e_box_p1) + (1<<e_label_p1) + (1<<e_p1_x) + (1<<e_p1_y ) \
 + (1<<e_box_p2) + (1<<e_label_p2) + (0<<e_p2_x) + (0<<e_p2_y ) \
 + (1<<e_label_hole) + (1<< e_hole ) \
 + (1<<e_label_thick) + (1<< e_thick ) \
 + (1<<e_label_clear) + (1<<e_clear ) \
 + (1<<e_label_mask ) + (1<<e_mask ) \
 + (1<<e_label_shape) + (1<<e_shape ) \
 + (1<<e_label_layer) + (1<<e_layer)
