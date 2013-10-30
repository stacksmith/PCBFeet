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
#include <malloc.h>

#include "view.h"
#include "pad.h"

sPad*pad;

sView* view_new(){
  return (sView*)malloc(sizeof(sView));
}
void view_initialize(sView* view,const char* uiname){
  
  view->origin.x=-10000;
  view->origin.y=-10000;
  view->scale = 100;

  view->grid_origin.x=0;
  view->grid_origin.y=0;
  view->grid_unit = 1000;
  
  view->mouse.x = 0;
  view->mouse.y = 0;
  
  view->mode = MODE_IDLE;
  
  //move this to document
  pad = pad_new();
  pad_init(pad);

  GtkBuilder *builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, uiname, NULL);
  //keep track of important widgets
  view->frame = (GtkWidget*)gtk_builder_get_object (builder, "frame");
  view->canvas = (GtkWidget*)gtk_builder_get_object (builder, "canvas");
  view->hruler = (GtkWidget*)gtk_builder_get_object (builder, "hruler");
  view->vruler = (GtkWidget*)gtk_builder_get_object (builder, "vruler");
  view->status_xy = (GtkLabel*)gtk_builder_get_object (builder, "status_xy");
  view->but_origin = (GtkWidget*)gtk_builder_get_object (builder, "but_origin");
//  gtk_drag_source_set(view->but_origin,GDK_BUTTON1_MASK,NULL,0,GDK_ACTION_PRIVATE);
  gtk_builder_connect_signals(builder,view);
  /* Exit when the window is closed */
  g_signal_connect (view->frame, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  //TODO: delete builder
  view->width = gtk_widget_get_allocated_width(view->canvas);
  view->height = gtk_widget_get_allocated_height(view->canvas);
}

/*****************************************************************************/
// Draw the grid
//
void grid_draw(GtkWidget *widget, cairo_t *cr, sView* view){
  cairo_set_antialias(cr,CAIRO_ANTIALIAS_NONE);
  cairo_set_source_rgb(cr, .5,.5,.5);
  cairo_set_line_width(cr, 1);
  int x,y;
  cairo_set_line_cap(cr,CAIRO_LINE_CAP_ROUND);
  // how many pixels per grid unit...
  int px_unit = view->grid_unit / view->scale; 
  //Draw the grid in view coordinates. 
  //Convert screen (0,0) to nearest grid coordinate...
 printf("grid_draw1 grid_origin (%d,%d), origin(%d,%d)\n",
   view->grid_origin.x, view->grid_origin.y,
   view->origin.x, view->origin.y);
  
  sPoint start = {
    ((view->grid_origin.x - view->origin.x)/view->scale),// /px_unit)*px_unit,
    ((view->grid_origin.y - view->origin.y)/view->scale)};// /px_unit)*px_unit };
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
/*****************************************************************************/
/*
 * */
extern gboolean canvas_draw_cb(GtkWidget *widget, cairo_t *cr, 
    sView* view){
printf("cadwin_draw %d %d\n",gtk_widget_get_allocated_width(widget),
  gtk_widget_get_allocated_height(widget));

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_identity_matrix(cr);
  //rulers
  
/*  cairo_set_line_width(cr, 1);
  cairo_move_to(cr,0,0);
  cairo_line_to(cr,1000,1000);
  cairo_stroke(cr);    
*/
  pad_draw(pad,cr,view);
  grid_draw(widget,cr,view);
  //draw the targeting lines
  int x,y;
  return FALSE;
}

/*****************************************************************************/
// set the status_xy text to match mouse coordinates in grid terms
//
void status_xy_update(sView* view){
  char buf[32];
  sprintf(buf,"(%5g,%5g)",
    (view->mouse.x-view->grid_origin.x)/100.0, //expressed as grid coordinate
    (view->mouse.y-view->grid_origin.y)/100.0);
  // show coordinates
  gtk_label_set_label((GtkLabel*)view->status_xy,buf);
  // redraw rulers
  gtk_widget_queue_draw(view->hruler); //redraw ruler
  gtk_widget_queue_draw(view->vruler); //redraw ruler  
}
/*****************************************************************************/
// As the mouse moves, update the ruler and the coordinates.
//
extern gboolean canvas_motion_notify_event_cb (GtkWidget *canvas,
                                            GdkEventMotion  *event,
                                            sView* view)  {
  // store mouse coordinates as view coordinates
  view->pxMouse.x = event->x;
  view->pxMouse.y = event->y;
  printf("canvas_motion_notify_event_cb (%d,%d)\n",
         view->pxMouse.x,view->pxMouse.y)  ;
  
  //TODO: remove this
  view->mouse.x = ((int)(event->x))*view->scale+ view->origin.x ;
  view->mouse.y = ((int)(event->y))*view->scale+ view->origin.y ;
  status_xy_update(view);
  //TODO: optimise targeting line redraw...
  gtk_widget_queue_draw(canvas); //redraw ruler
  return FALSE;
}
/*****************************************************************************/
extern gboolean canvas_configure_event_cb(GtkWidget* canvas,GdkEventConfigure* event,sView* view){
 printf("resized is %d\n",event->width);
  view->width = event->width;
  view->height = event->height;
  return FALSE;
  
}
/*****************************************************************************/
// Scale the view
// Scaling happens around the current mouse pointer location...
extern gboolean canvas_scroll_event_cb(GtkWidget* canvas,GdkEventScroll* event,sView* view){
  GdkScrollDirection dir = event->direction;
//  printf("SCROLLing %f,",view->scale);
  float newscale;
  int pix_x =  (view->mouse.x - view->origin.x)/view->scale;
  int pix_y =  (view->mouse.y - view->origin.y)/view->scale;
//  int pix_x =  view->grid_origin.x/view->scale - view->origin.x/view->scale;
  switch(dir){
    case GDK_SCROLL_UP: 
      newscale = view->scale*.9;
      if(newscale<.1) newscale=.1;
      break;
    case GDK_SCROLL_DOWN: 
      newscale = view->scale*1.1;
      break;
    default: return FALSE;
  }
  view->origin.x += (pix_x*view->scale - pix_x*newscale);    
  view->origin.y += (pix_y*view->scale - pix_y*newscale);    
  view->scale = newscale;
  //adjust the view origin so that the mouse is in the same place
  
  gtk_widget_queue_draw(canvas);
  printf("SCROLLED %f\n",view->scale);
  gtk_widget_queue_draw(view->hruler); //redraw ruler
  gtk_widget_queue_draw(view->vruler); //redraw ruler
  return FALSE;
}
/*****************************************************************************/
gboolean canvas_button_release_event_cb(GtkWidget* canvas,GdkEventButton* event,sView* view){
//  printf("canvas_button_release_event_cb %d %d\n",(int)event->x,(int)event->y);
//  printf("canvas_button_release_event_cb %f %f\n",event->x,event->y);
  printf("canvas_button_release_event_cb %d %d\n",(int)event->x,(int)event->y);
  GdkWindow* w = gtk_widget_get_window(view->frame);
  GdkCursor* cur = gdk_cursor_new(GDK_LEFT_PTR);
  gdk_window_set_cursor(w,cur);
  view->mode = MODE_IDLE;  //mode switch  

int old = view->grid_origin.x;
//  getchar();
  view->grid_origin.x = view->mouse.x;
  view->grid_origin.y = view->mouse.y;

printf("from %d to %d\n",
         old, view->grid_origin.x);
  //force updates of canvas (to redraw grid) and rulers (since origin changed)
  gtk_widget_queue_draw(canvas); 
  gtk_widget_queue_draw(view->hruler); //redraw ruler
  gtk_widget_queue_draw(view->vruler); //redraw ruler
  status_xy_update(view);
  return FALSE;
  
}
/*****************************************************************************/
void vbar_adj_value_changed_cb(GtkAdjustment* adj,gpointer user_data){
  printf("vert adj: %f\n",gtk_adjustment_get_value(adj));
}
/*****************************************************************************/
void hbar_adj_value_changed_cb(GtkAdjustment* adj,gpointer user_data){
  printf("vert adj: %f\n",gtk_adjustment_get_value(adj));
}
