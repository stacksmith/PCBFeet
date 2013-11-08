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
  label_frame,   
  label_name,  name,
  label_number,  number,
  //
  box_p1,  label_p1,  p1_x,  p1_y,
  box_p2,  label_p2,  p2_x,  p2_y,
  label_hole,  hole,
  //
  label_thick,  thick,
  label_clear,  clear,
  label_mask,  mask,
  //
  label_shape,  shape,
  label_layer,  layer,
  pw_last
} eParmWidget;

static char* parm_widget_name[]={
  "parm_label_frame","parm_label_name", "parm_name", "parm_label_number", "parm_number",
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


