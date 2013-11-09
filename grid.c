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
#include <math.h>
#include "types.h"
#include "view.h"
/*****************************************************************************/
// Draw the grid
//

void grid_calc(GtkWidget *canvas, cairo_t *cr, sView* view){
  
}
void grid_draw(GtkWidget *canvas, cairo_t *cr, sView* view){
  sGrid* grid = &view->grid;
//  cairo_set_antialias(cr,CAIRO_ANTIALIAS_NONE);
  cairo_set_source_rgb(cr, 0,0,0);
  cairo_set_line_width(cr, 1);
  int x,y;
  cairo_set_line_cap(cr,CAIRO_LINE_CAP_ROUND);
  // how many pixels per grid unit...
  int px_unit = grid->unit / view->scale; 
  //Draw the grid in view coordinates. 
  //Convert screen (0,0) to nearest grid coordinate...
//printf("grid_draw1 grid_origin (%d,%d), origin(%d,%d)\n",
//   grid->origin.x, grid->origin.y,
//   view->origin.x, view->origin.y);
  
  int w=0;
  int h=0;
  sPoint start = {
    ((grid->origin.x - view->origin.x)/view->scale),// /px_unit)*px_unit,
    ((grid->origin.y - view->origin.y)/view->scale)};// /px_unit)*px_unit };
//printf("grid_draw1 start (%d,%d),%d\n",start.x,start.y,px_unit);
  //draw the four quadrants
  for(y=start.y; y<view->height; y+=px_unit){   //lower
    for(x=start.x; x<view->width; x+=px_unit){  //right
      cairo_move_to(cr, x+.5,y+.5);
      cairo_line_to(cr, x+.5,y+.5);
      w++;h++;
    }
    for(x=start.x; x>=0; x-=px_unit){            //lower left
      cairo_move_to(cr, x+.5,y+.5);
      cairo_line_to(cr, x+.5,y+.5);
      w++;h++;
    } 
  } 
  grid->pxOrigin.x = x+px_unit;          //set grid's leftmost position
  for(y=start.y; y>=0; y-=px_unit){
    for(x=start.x; x<view->width; x+=px_unit){
      cairo_move_to(cr, x+.5,y+.5);
      cairo_line_to(cr, x+.5,y+.5);
      w++;h++;
    }
    for(x=start.x; x>=0; x-=px_unit){
      cairo_move_to(cr, x+.5,y+.5);
      cairo_line_to(cr, x+.5,y+.5);
      w++;h++;
    }
  }
  grid->pxOrigin.y = y+px_unit;          //set grid's topmost position
  grid->xyCnt.x = w; //set grid units count in w and h
  grid->xyCnt.y = h;
//  cairo_stroke(cr);    
//targeting lines
  // This is a little tricky.  The mouse cursor moves freely, and when it crosses the midpoint
  // between gridlines, we switch to the nearest gridline.  Account for left grid margin...
  grid->nearest.point.x = ((view->pxMouse.x-grid->pxOrigin.x + px_unit/2) / px_unit) * px_unit + grid->pxOrigin.x;
  grid->nearest.point.y = ((view->pxMouse.y-grid->pxOrigin.y + px_unit/2 ) / px_unit)* px_unit+ grid->pxOrigin.y;
  grid->nearest.distance = 
    sqrt(pow((view->pxMouse.x-grid->nearest.point.x),2)+(pow((view->pxMouse.y-grid->nearest.point.y),2)));
//printf("grid_draw2 nearest(%d %d) distance %f\n",grid->nearest.point.x,grid->nearest.point.y,
//  grid->nearest.distance);

  cairo_stroke(cr);    
//  cairo_set_antialias(cr,CAIRO_ANTIALIAS_NONE);

  cairo_move_to(cr,grid->nearest.point.x+.5 ,0.5);
  cairo_line_to(cr,grid->nearest.point.x+.5,view->height+.5);
  cairo_move_to(cr,0+.5,grid->nearest.point.y+.5);
  cairo_line_to(cr,view->width+.5,grid->nearest.point.y+.5);

//  cairo_move_to(cr,view->pxMouse.x,0);
//  cairo_line_to(cr,view->pxMouse.x,view->height);
//  cairo_move_to(cr,0,y=view->pxMouse.y);
//  cairo_line_to(cr,view->width,view->pxMouse.y);
  cairo_stroke(cr);    
// printf("grid_draw2 %d %d\n",view->pxMouse.x,view->pxMouse.y);
}
/*****************************************************************************/
// Find the nearest gridpoint...
void grid_nearest_point(sNearestPoint* result, sView* view,sPoint* here){
  sGrid* grid = &view->grid;
  int px_unit = grid->unit / view->scale;
//  xind = (here->x - grid->pxOrigin.x) / px_unit;
//  yind = (here->y - grid->pxOrigin.y) / px_unit;
  
  
}