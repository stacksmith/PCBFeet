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
#include <cairo.h>
#include <gtk/gtk.h>
#include <stdio.h>

#include "types.h"
#include "view.h"
#include "element.h"


/*****************************************************************************/

int main(int argc, char *argv[])
{
  gtk_init(&argc, &argv);

  sView* view = view_new();
  sElement* el = element_new();
  element_init(el);
  view->element = el;
  view_initialize(view,"fcad.ui");
  
  gtk_main();

  return 0;
}

