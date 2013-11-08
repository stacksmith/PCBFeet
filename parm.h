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
typedef struct sParm {
  GtkLabel* label_frame;   //name of the entire parameter space  
  GtkLabel* label_name;
  GtkEntry* name;
  GtkLabel* label_number;
  GtkEntry* number;
  //
  GtkBox* box_p1;
  GtkBox* box_p2;  
  GtkLabel* label_p1;
  GtkEntry* p1_x;
  GtkEntry* p1_y;
  GtkLabel* label_p2;
  GtkEntry* p2_x;
  GtkEntry* p2_y;
  GtkLabel* label_hole;
  GtkEntry* hole;
  //
  GtkLabel* label_thick;
  GtkEntry* thick;
  GtkLabel* label_clear;
  GtkEntry* clear;
  GtkLabel* label_mask;
  GtkEntry* mask;
  //
  GtkLabel* label_shape;
  GtkComboBoxText* shape;
  GtkLabel* label_layer;
  GtkComboBoxText* layer;
} sParm;