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
#include "ruler.h"
#define HRULER_WIDTH 16

/*  Ruler is drawn in pixels (there is always an integral centimils in a pix)
 * To determine the ruler unit, we attempt to match the grid with marks; if
 * the marks are more then 10 pixels, we continue subdividing.
*/
extern gboolean vruler_draw_cb(GtkWidget *widget, cairo_t *cr, sView* view){
  return FALSE;
}
/*****************************************************************************/
extern gboolean origin_draw_cb(GtkWidget *widget, cairo_t *cr, sView* view){
  return FALSE;
}
/*****************************************************************************/
//TODO: move this to a better place
void but_origin_clicked_cb(GtkButton* but,sView* view){
  printf("but_origin_clicked_cb\n");
  GdkWindow* w = gtk_widget_get_window(view->frame);
  GdkCursor* cur = gdk_cursor_new(GDK_TOP_LEFT_CORNER);
  gdk_window_set_cursor(w,cur);
  view->mode = MODE_SET_ORIGIN;  //mode switch  
  //TODO: is cursor deleted?
}

/*****************************************************************************/
// primitive for labelling rulers.  
// -set up the font previously...
// -call cairo_move_to the start position
void ruler_label(cairo_t* cr,double value){
  char buf[32];
  sprintf(buf,"%g",value);
  cairo_show_text(cr, buf);  
}
/*****************************************************************************/
extern gboolean hruler_draw_cb(GtkWidget *widget, cairo_t *cr, sView* view){
//decide on a notch value.  Start with grid
  int unit = view->grid.unit / view->scale; //in pixels

   cairo_select_font_face(cr, "Sans",
      CAIRO_FONT_SLANT_NORMAL,
      CAIRO_FONT_WEIGHT_NORMAL);    
printf("hruler_draw_cb %d\n",unit);
cairo_set_font_size(cr, 8);
    cairo_move_to(cr,0,10); //long stroke
  cairo_identity_matrix(cr);
  cairo_set_line_cap(cr,CAIRO_LINE_CAP_SQUARE);
  cairo_set_source_rgb(cr, 0,0,0);
  cairo_set_line_width(cr, 1);
  cairo_set_antialias(cr,CAIRO_ANTIALIAS_NONE);
  //Ruler is empirically matched to the grid.  Draw from origin...
  int px_unit = view->grid.unit / view->scale; 
  int start =  (view->grid.origin.x - view->origin.x)/view->scale;
  double value = 0;
  int i;
  for(i=start;i<view->width;i+=unit*5,value+=view->grid.unit*5){
//printf("hruler (%d,%d)\n",i,HRULER_WIDTH);
    cairo_move_to(cr,i,HRULER_WIDTH); //long stroke
    cairo_line_to(cr,i,HRULER_WIDTH-6);
    ruler_label(cr,value/100.0);
    unsigned int j;
    for(j=unit; j<unit*5;j+=unit){
      cairo_move_to(cr,i+j,HRULER_WIDTH);  //short strokes
      cairo_line_to(cr,i+j,HRULER_WIDTH-3);
    }
  }
  value=0;
  for(i=start;i>0;i-=unit*5,value-=view->grid.unit*5){
//printf("hruler (%d,%d)\n",i,HRULER_WIDTH);
    cairo_move_to(cr,i,HRULER_WIDTH); //long stroke
    cairo_line_to(cr,i,HRULER_WIDTH-6);
    ruler_label(cr,value/100.0);
    unsigned int j;
    for(j=unit; j<unit*5;j+=unit){
      cairo_move_to(cr,i-j,HRULER_WIDTH);  //short strokes
      cairo_line_to(cr,i-j,HRULER_WIDTH-3);
    }
  }
  // convert native mils to pixels
  cairo_move_to(cr,HRULER_WIDTH,0);
  cairo_move_to(cr,view->width,0);
  // current position
  int x = view->pxMouse.x;
  cairo_move_to(cr,x,0);
  cairo_line_to(cr,x,HRULER_WIDTH);

  cairo_stroke(cr);
  return FALSE;//true maybe
}
