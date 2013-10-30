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
#include "view.h"
/*****************************************************************************/
// Draw the grid
//
void grid_draw(GtkWidget *canvas, cairo_t *cr, sView* view){
  cairo_set_antialias(cr,CAIRO_ANTIALIAS_NONE);
  cairo_set_source_rgb(cr, .5,.5,.5);
  cairo_set_line_width(cr, 1);
  int x,y;
  cairo_set_line_cap(cr,CAIRO_LINE_CAP_ROUND);
  // how many pixels per grid unit...
  int px_unit = view->grid.unit / view->scale; 
  //Draw the grid in view coordinates. 
  //Convert screen (0,0) to nearest grid coordinate...
 printf("grid_draw1 grid_origin (%d,%d), origin(%d,%d)\n",
   view->grid.origin.x, view->grid.origin.y,
   view->origin.x, view->origin.y);
  
  sPoint start = {
    ((view->grid.origin.x - view->origin.x)/view->scale),// /px_unit)*px_unit,
    ((view->grid.origin.y - view->origin.y)/view->scale)};// /px_unit)*px_unit };
 printf("grid_draw1 start (%d,%d),%d\n",start.x,start.y,px_unit);
  //draw the four quadrants
  for(y=start.y; y<view->height; y+=px_unit){
    for(x=start.x; x<view->width; x+=px_unit){
          cairo_move_to(cr, x-1, y);
          cairo_line_to(cr, x,y);
    }
    for(x=start.x; x>0; x-=px_unit){
          cairo_move_to(cr, x-1, y);
          cairo_line_to(cr, x,y);
    } 
  } 
  for(y=start.y; y>0; y-=px_unit){
    for(x=start.x; x<view->width; x+=px_unit){
          cairo_move_to(cr, x-1, y);
          cairo_line_to(cr, x,y);
    }
    for(x=start.x; x>0; x-=px_unit){
          cairo_move_to(cr, x-1, y);
          cairo_line_to(cr, x,y);
  } }
  
//  cairo_stroke(cr);    
//targeting lines
  cairo_move_to(cr,view->pxMouse.x,0);
  cairo_line_to(cr,view->pxMouse.x,view->height);
  cairo_move_to(cr,0,y=view->pxMouse.y);
  cairo_line_to(cr,view->width,view->pxMouse.y);
  cairo_stroke(cr);    
 printf("grid_draw2 %d %d\n",view->pxMouse.x,view->pxMouse.y);
}
