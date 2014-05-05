/*                                                                                                                                                                                                                   
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

#define __DYNAMIC_LIST_C
#include <dynamicList.h>

/**
 * Create a dynamic list instance
 * @return dynamic list pointer or NULL
 */
dynamicList *dynamicList_new() {
  struct dynamicList *dl=calloc(sizeof(dynamicList),1);
  if (!dl) {
    return NULL;
  }
  dl->push=dynamicList_push;
  dl->pop=dynamicList_pop;
  dl->shift=dynamicList_shift;
  dl->set=dynamicList_set;
  dl->get=dynamicList_get;
  dl->insert=dynamicList_insert;
  dl->remove=dynamicList_remove;
  return dl;
}

/**
 * Append item to list
 * @param list the list
 * @param data the item data pointer
 * @return the list item pointer or NULL on error
 */
dynamicListItem *dynamicList_push(dynamicList *list,void *data) {
  dynamicListItem *item=calloc(sizeof(dynamicListItem),1);
  if (!item) {
    return NULL;
  }
  item->data=data;
  if (!list->first) {
    list->first=item;
    list->last=item;
  } else {
    list->last->next=item;
    item->prev=list->last;
    list->last=item;
  }
  ++list->length;
  return item;
}

/**
 * Return the last item and remove it
 * @param list the list
 * @return the item data pointer or NULL on error
 */
void *dynamicList_pop(dynamicList *list) {
  dynamicListItem *item;
  void *data;
  if (!list->last) {
    return NULL;
  }
  item=list->last;
  if (item->prev) {
    list->last=item->prev;
    list->last->next=NULL;
  } else {
    list->first=NULL;
    list->last=NULL;
  }
  --list->length;
  data=item->data;
  free(item);
  return data;
}

/**
 * Return the first item and remove it
 * @param list the list
 * @return the item data pointer or NULL on error
 */
void *dynamicList_shift(dynamicList *list) {
  dynamicListItem *item;
  void *data;
  if (!list->first) {
    return NULL;
  }
  item=list->first;
  if (item->next) {
    list->first=item->next;
    list->first->prev=NULL;
  } else {
    list->first=NULL;
    list->last=NULL;
  }
  --list->length;
  data=item->data;
  free(item);
  return data;
}

/**
 * insert an item before or after the specified index
 * @param list the list
 * @param index the item index
 * @param after insert after (boolean)
 * @param data the item data pointer 
 * @return the item data or NULL on error
 */
dynamicListItem *dynamicList_insert(dynamicList *list, unsigned long index, int after, void *data) {
  dynamicListItem *item;
  dynamicListItem *itemPtr;
  if (index>=list->length) {
    return NULL;
  }
  item=calloc(sizeof(dynamicListItem),1);
  if (!item) {
    return NULL;
  }
  item->data=data;
  
  for(itemPtr=list->first;index;--index) {
    itemPtr=itemPtr->next;
  }

  if (after) {
    if (itemPtr->next) {
      item->next=itemPtr->next;
      item->next->prev=item;
    } else {
      list->last=item;
      itemPtr->next=item;
    }
    item->prev=itemPtr;
    item->prev->next=item;

  } else  {
    if (itemPtr->prev) {
      item->prev=itemPtr->prev;
      item->prev->next=item;
    } else {
      list->first=item;
      itemPtr->prev=item;
    }
    item->next=itemPtr;
    item->next->prev=item;
  }

  ++list->length;
  return item;
}

/**
 * Copy the specified item at the specified location
 * @param list the list
 * @param index the item index
 * @param data the item data pointer
 * @return the list item pointer or NULL on error
 */
dynamicListItem *dynamicList_set(dynamicList *list,unsigned long index,void *data) {
  dynamicListItem *item;
  if (index>=list->length) {                 
    return NULL;                                                       
  }
  for(item=list->first; index; --index) {
    item=item->next;
  }
  item->data=data;
  return item;
}

/**
 * Get the specified item pointer
 * @param list the list
 * @param index the item index
 * @return the list item pointer or NULL on error
 */
dynamicListItem *dynamicList_get(dynamicList *list,unsigned long index) {
  dynamicListItem *item;
  if (index>=list->length) {
    return NULL;
  }
  for(item=list->first; index; --index) {
    item=item->next;
  }
  return item;
}

/**
 * Delete the specified item pointer
 * @param list the list
 * @param index the item index
 * @param dataDispose the item data destructor (can return non-zero to cancel removal from list)
 * @return 1 on error 0 otherwise
 */
int dynamicList_remove(dynamicList *list,unsigned long index,int (*dataDispose)(void *data)) {
  dynamicListItem *item;
  if (index>=list->length) {
    return -1;
  }
  for(item=list->first; index; --index) {
    item=item->next;
  }
  if (dataDispose) {
    if (dataDispose(item->data)) {
      return 1;
    }
  }
  if (item->prev) {
    item->prev->next=item->next;
    if (!item->next) {
      list->last=item->prev;
    }
  } else {
    list->first=item->next;
  }
  if (item->next) {
    item->next->prev=item->prev;
    if (!item->prev) {
      list->first=item->next;
    }
  } else {
    list->last=item->prev;
  }
  --list->length;
  return 0;
}
