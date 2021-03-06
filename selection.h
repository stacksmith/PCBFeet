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

/******************************************************************************
 Keep track of current selection, and act upon any changes
*/
typedef struct sObject sObject;

typedef struct sSelection {
    GSList*      data;            //a linked list of data parts
} sSelection;

void selection_add(sSelection* sel,sObject* object);
void selection_clear(sSelection* sel);