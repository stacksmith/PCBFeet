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

#include "types.h"
#include "view.h"

//sPad*pad;
#include "element.h"
#include "document.h"
sView* view_new(){
  return (sView*)g_malloc0(sizeof(sView));
}
//TODO: get rid of this
extern GtkWidget* pin_create_config();

void view_initialize(sView* view,const char* uiname){
  
  view->origin.x=-10000;
  view->origin.y=-10000;
  view->scale = 100;

  view->grid.origin.x=0000;
  view->grid.origin.y=0000;
  view->grid.unit = 1000;
  
  view->pxMouse.x = 0;
  view->pxMouse.y = 0;
  
  view->mode_set_origin = FALSE;
  
  //move this to document
//  pad = pad_new();
//  pad_init(pad);

  GtkBuilder *builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, uiname, NULL);
  //keep track of important widgets
  view->frame = (GtkWidget*)gtk_builder_get_object (builder, "frame");
  view->canvas = (GtkWidget*)gtk_builder_get_object (builder, "canvas");
  view->hruler = (GtkWidget*)gtk_builder_get_object (builder, "hruler");
  view->vruler = (GtkWidget*)gtk_builder_get_object (builder, "vruler");
  view->status_xy = (GtkLabel*)gtk_builder_get_object (builder, "status_xy");
  view->but_origin = (GtkWidget*)gtk_builder_get_object (builder, "but_origin");
  view->objects  =   (GtkWidget*)gtk_builder_get_object (builder, "objects");
printf("=================%p\n",view->objects);
 
  GtkBuilder *builder1 = gtk_builder_new ();
  gtk_builder_add_from_file (builder1, "test.ui", NULL);
GtkWidget* framex =(GtkWidget*)gtk_builder_get_object (builder1, "frame1"); 
// if adding a windowless hierarchy, use this
//  gtk_container_add((GtkContainer*)view->objects,framex);
// But to edit with glade, we put it into a window, so reparent
gtk_widget_reparent(framex,(GtkContainer*)view->objects);

//  gtk_container_add((GtkContainer*)view->objects,pin_create_config());
//  gtk_drag_source_set(view->but_origin,GDK_BUTTON1_MASK,NULL,0,GDK_ACTION_PRIVATE);
  gtk_builder_connect_signals(builder,view);
  /* Exit when the window is closed */
  g_signal_connect (view->frame, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  gtk_widget_show_all(view->frame);
  //TODO: delete builder
  view->width = gtk_widget_get_allocated_width(view->canvas);
  view->height = gtk_widget_get_allocated_height(view->canvas);
  
  
}

/*****************************************************************************/
/*
 * */
extern gboolean canvas_draw_cb(GtkWidget *widget, cairo_t *cr, 
    sView* view){


  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_identity_matrix(cr);
  //rulers
  
/*  cairo_set_line_width(cr, 1);
  cairo_move_to(cr,0,0);
  cairo_line_to(cr,1000,1000);
  cairo_stroke(cr);    
*/
//  pad_draw(pad,cr,view);
printf("cadwin_draw %p\n",view);
printf("cadwin_draw %p\n",view->document);
printf("cadwin_draw %p\n",view->document->element);
  element_draw(view->document->element,cr,view);
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
    (view->pxMouse.x*view->scale+view->origin.x - view->grid.origin.x)/100.0, //expressed as grid coordinate
    (view->pxMouse.y*view->scale+view->origin.y - view->grid.origin.y)/100.0);
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
//  view->mouse.x = ((int)(event->x))*view->scale+ view->origin.x ;
//  view->mouse.y = ((int)(event->y))*view->scale+ view->origin.y ;
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
  int pix_x =  view->pxMouse.x;
  int pix_y =  view->pxMouse.y;
//  int pix_x =  view->grid.origin.x/view->scale - view->origin.x/view->scale;
  switch(dir){
    case GDK_SCROLL_UP: 
      newscale = view->scale-(view->scale/10+1);//*.9;
      if(newscale<1) newscale=1;
      break;
    case GDK_SCROLL_DOWN: 
      newscale = view->scale+(view->scale/10+1);//1.1;
      break;
    default: return FALSE;
  }
  view->origin.x += (pix_x*view->scale - pix_x*newscale);    
  view->origin.y += (pix_y*view->scale - pix_y*newscale);    
  view->scale = newscale;
  //adjust the view origin so that the mouse is in the same place
  
  gtk_widget_queue_draw(canvas);
  printf("SCROLLED %d\n",view->scale);
  gtk_widget_queue_draw(view->hruler); //redraw ruler
  gtk_widget_queue_draw(view->vruler); //redraw ruler
  return FALSE;
}
/*****************************************************************************/
gboolean canvas_button_release_event_cb(GtkWidget* canvas,GdkEventButton* event,sView* view){
  printf("canvas_button_release_event_cb %d %d\n",(int)event->x,(int)event->y);
  if(view->mode_set_origin){
    GdkWindow* w = gtk_widget_get_window(view->frame);
    GdkCursor* cur = gdk_cursor_new(GDK_LEFT_PTR);
    gdk_window_set_cursor(w,cur);
    view->mode_set_origin = FALSE;  //mode switch  
int old = view->grid.origin.x;
    view->grid.origin.x = view->pxMouse.x*view->scale+view->origin.x;
    view->grid.origin.y = view->pxMouse.y*view->scale+view->origin.y;

printf("from %d to %d\n", old, view->grid.origin.x);
  //force updates of canvas (to redraw grid) and rulers (since origin changed)
    gtk_widget_queue_draw(canvas); 
    gtk_widget_queue_draw(view->hruler); //redraw ruler
    gtk_widget_queue_draw(view->vruler); //redraw ruler
    status_xy_update(view);
  } else {
    // Regular click-release.  Locate object under mouse
    printf("FUCK\n");
    element_hit_test(view->document->element,view);
  }
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

/*****************************************************************************/

void but_origin_clicked_cb(GtkButton* but,sView* view){
  printf("but_origin_clicked_cb\n");
  GdkWindow* w = gtk_widget_get_window(view->frame);
  GdkCursor* cur = gdk_cursor_new(GDK_TOP_LEFT_CORNER);
  gdk_window_set_cursor(w,cur);
  view->mode_set_origin=TRUE;  //makes mouse click set origin 
}

