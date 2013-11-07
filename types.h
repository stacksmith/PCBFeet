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
/* some common types
*/
#pragma once
typedef int coord;

typedef struct sPoint{
  coord x;
  coord y;
} sPoint;

// sNearestPoint is used for measurements
typedef struct sNearestPoint{
  sPoint point; //nearest point (usually for grid)
//  sPoint delta; //x and y distances to nearest point
  double distance; //as the crow flies
} sNearestPoint;

#include <stdio.h>
#include <setjmp.h>

#define TRY do{ jmp_buf ex_buf__; if( !setjmp(ex_buf__) ){
#define CATCH } else {
#define ENDTRY } }while(0);
#define THROW longjmp(ex_buf__, 1)


