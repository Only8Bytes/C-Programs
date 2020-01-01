#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csapp.h"
#include "cache.h"

/* cache_init initializes the input cache linked list. */
void cache_init(CacheList *list) {
  list->size = 0;
  list->first = NULL;
  list->last = NULL;
}

/* cache_URL adds a new cached item to the linked list. It takes the
 * URL being cached, a link to the content, the size of the content, and
 * the linked list being used. It creates a struct holding the metadata
 * and adds it at the beginning of the linked list.
 */
void cache_URL(const char *URL, const char *headers, void *item, size_t size, CacheList *list) {
  //make sure the object isn't too big for the cache
  if (size > MAX_OBJECT_SIZE){
    //we can free the object since we can't cache it
    free(item);
    return;
  }
  //remove least used nodes until we have space, don't remove any if we have enough space
  while (size + list->size > MAX_CACHE_SIZE){
    //evict the last node
    //assume tail is not NULL as the cache is full
    CachedItem *tmp = list->last;
    list->size = (list->size) - (tmp->size);
    list->last = (list->last)->prev;
    (list->last)->next = NULL;
    free(tmp->item_p);
    free(tmp);
  }
  //add new node
  CachedItem *newNode = Malloc(sizeof(CachedItem));
  newNode->url = strdup(URL);
  newNode->headers = strdup(headers);
  newNode->item_p = item;
  newNode->size = size;
  list->size = (list->size) + size;
  //add node to start of linked list
  newNode->next = list->first;
  newNode->prev = NULL;
  list->first = newNode;
  if (list->last == NULL){
    list->last = newNode;
  }
}


/* find iterates through the linked list and returns a pointer to the
 * struct associated with the requested URL. If the requested URL is
 * not cached, it returns null.
 */
CachedItem *find(const char *URL, CacheList *list) {
  CachedItem *current = list->first;
  if (current->url == URL){
    //found the url in the first CachedItem
    return current;
  }
  //continue until the end of the list
  while (current->next != NULL){
    current = current->next;
    if (current->url == URL){
      //found the url in current CachedItem
      //rio_writen(fd, current->responseHeaders, strlen(current->responseHeaders));
      //rio_writen(fd, current->ptr, current->size);
      if (list->last == current){
        //remove current CachedItem from end of the list
        list->last = current->prev;
        (current->prev)->next = NULL;
        current->prev = NULL;
      } else {
        //remove current CachedItem from middle of the list
        (current->prev)->next = current->next;
        (current->next)->prev = current->prev;
        current->prev = NULL;
        current->next = NULL;
      }
      //add current CachedItem to front of the list
      current->next = list->first;
      (list->first)->prev = current;
      list->first = current;
      return current;
    }
  }
  return NULL;
}


/* frees the memory used to store each cached object, and frees the struct
 * used to store its metadata. */
void cache_destruct(CacheList *list) {
  CachedItem *tmp;
  //for every node in the list set the first node to the following one and remove all pointers to it
  //then free it from the memory
  while (list->first != NULL){
    tmp = list->first;
    (list->first) = (list->first)->next;
    free(tmp->item_p);
    free(tmp); 
  }
  list->last = NULL;
}
