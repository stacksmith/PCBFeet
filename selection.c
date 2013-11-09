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
#include "selection.h"
#include "object.h"
#include "parm.h" //for mask
/******************************************************************************
 Keep track of current selection, and act upon any changes
*/

void selection_init(sSelection* sel){
  sel->data=NULL;
}

void selection_clear(sSelection* sel,sView* view){
  g_slist_free(sel->data);
}

void selection_add(sSelection* sel,sObject* object){
  sel->data = g_slist_prepend(sel->data,object);
}

/******************************************************************************
 Calculate which UI elements are used in the config area for the selection.
 Single items will get the full treatment; multiple selections get the
 intersecting elements only.  
*/
unsigned int selection_uimask(sSelection* sel){
  GSList* p = sel->data;
  unsigned int result = 0xFFFFFFFF;
  int i=0;
  while(p){
printf("selection_uimask %x\n",result);
printf("selection_uimask ((sObject*)(p->data))->UIMask is %x\n",((sObject*)(p->data))->UIMask);
    result &= ((sObject*)(p->data))->UIMask;  //and the masks for common
    p= g_slist_next(p);
    i++;
  }
  if(i>1) result &= UIMASK_MULTIPLE;//multiple selection turns off name/num
printf("selection_uimask RESULT: %x\n",result);
  return result;
}

