 *   dynamicList
 *
 *   Copyright (C) 2014 Foxel SA
 *               
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *               
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *               
 *   You should have received a copy of the GNU General Public Licenseu
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Author: Luc Deschenaux
 *
 */

#ifndef __DYNAMIC_LIST__
#define __DYNAMIC_LIST__

#ifdef __DYNAMIC_LIST_C__
#define __extern
#else
#define __extern extern
#endif

#include <string.h>
#include <stdlib.h>

typedef struct dynamicList dynamicList;
typedef struct dynamicListItem dynamicListItem;

struct dynamicListItem {
  void *data;
  dynamicListItem *next;
  dynamicListItem *prev;
};

struct dynamicList {
  dynamicListItem *first;
  dynamicListItem *last;
  size_t length;
  dynamicListItem *(*push)(dynamicList *list,void *data);
  void *(*pop)(dynamicList *list);
  void *(*shift)(dynamicList *list);
  dynamicListItem *(*set)(dynamicList *list,unsigned long index,void *data);
  dynamicListItem *(*get)(dynamicList *list,unsigned long index);
  dynamicListItem *(*insert)(dynamicList *list,unsigned long index, int after, void *data);
  int (*remove)(dynamicList *list,unsigned long index,int (*dataDispose)(void *data));
};

__extern dynamicList *dynamicList_new();
__extern dynamicListItem *dynamicList_push(dynamicList *list,void *data);
__extern void *dynamicList_pop(dynamicList *list);
__extern void *dynamicList_shift(dynamicList *list);
__extern dynamicListItem *dynamicList_set(dynamicList *list,unsigned long index,void *data);
__extern dynamicListItem *dynamicList_get(dynamicList *list,unsigned long index);
__extern dynamicListItem *dynamicList_insert(dynamicList *list,unsigned long index,int after,void *data);
__extern int dynamicList_remove(dynamicList *list,unsigned long index,int (*dataDispose)(void *data));

#undef __extern

#endif


