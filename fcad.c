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
#include "vtab.h"
#include "pad.h"
#include "pin.h"
#include "line.h"

int main(int argc, char *argv[])
{
  gtk_init(&argc, &argv);

  sView* view = view_new();
  sElement* el = element_new();
  element_init(el);
//  element_add(el,pad_new(1000,1000,2000,1500,  500,1000,1500,  "PadName","1",""));
  element_add(el,pin_new(0,0,8100,3937,8494,4600,"1","1",PIN_SQUARE));
  element_add(el,pin_new(10000,0,8100,3937,8494,4600,"2","2",PIN_ROUND));
  element_add(el,line_new(-4921, 4921,-4921,-4921,1000));
  element_add(el,line_new(-4921,-4921,14764,-4921,1000));
  element_add(el,line_new(14764,-4921,14764, 4921,1000));
  element_add(el,line_new(14764, 4921,-4921, 4921,1000));

  element_add(el,pad_new(10000,10000,20000,10000, 10000,20000,45000,"3","3",PAD_ROUND));
 // element_add(el,pin_new(4000,4000, 1000,1000,1500, 500, "Pin1","1",""));
  
  view->element = el;
  view_initialize(view,"fcad.ui");
  
  gtk_main();

  return 0;
}

