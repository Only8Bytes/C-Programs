/*
 * dict.c for CMPSC 311 Fall 2019
 * last updated: 10/15/2019
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"

// count number of keys in a dict.
int countKeys(const dictNode *dict) {
  int i = 0;
  const dictNode *current = dict;
  while (current != NULL){
    i += 1;
    current = current->next;
  } 
  return i;
}

// given a key, look up its corresponding value in the
// dictionary, returns -1 if the value is not in the dictionary.
// your search for key should end when the key in the next node
// is bigger than the lookup key or you reached the end of the
// list.
int lookupKey(const dictNode *dict, const char *key) {
  const dictNode *current = dict;
  while (current != NULL){
    int cmpVal = strcmp(current->key, key);
    if (cmpVal > 0){
      break;
    }
    if (cmpVal == 0){
      return current->value;
    }
    current = current->next;
  }
  return -1;
}

// delete the node in a dict with given key, return the value of
// the deleted node if it was found, return -1 if it wasn't found.
int deleteKey(dictNode **dict, const char *key) {
  int keyFound = lookupKey(*dict, key);
  if (keyFound == -1){
    return -1;
  }
  dictNode *current = *dict;
  if (strcmp(current->key, key) == 0){
    int returnVal = current->value;
    *dict = current->next;
    free(current);
    return returnVal;
  } 
  while (current != NULL){
    if (current->next == NULL){
      break;
    }
    if (strcmp(current->next->key, key) == 0){
      dictNode *tempNode = current->next;
      int returnVal = tempNode->value;
      current->next = tempNode->next;
      free(tempNode->key);
      free(tempNode);
      return returnVal;
    }
   current = current->next; 
  }
  return -1;
}

// given a key/value pair, first lookup the key in the dictionary,
// if it is already there, update the dictionary with the new
// value; if it is not in the dictionary, insert a new node into
// the dictionary, still make sure that the key is in alphabetical
// order.
// IMPORTANT: When creating a new node, make sure you dynamically
// allocate memory to store a copy of the key in the memory. You
// may use strdup function. DO NOT STORE the input key from the 
// argument directly into the node. There is no guarantee that key
// pointer's value will stay the same. 
// YOU MUST KEEP THE ALPHABETICAL ORDER OF THE KEY in the dictionary.
void addKey(dictNode **dict, const char *key, int value) {
  int keyFound = lookupKey(*dict, key);
  dictNode *current = *dict;
  char *keyCopy = strdup(key);
  if (keyFound == -1){
    /*not in dictionary*/
    dictNode *tempNode = malloc(sizeof(dictNode));
    tempNode->key = keyCopy;
    tempNode->value = value;
    if ((current == NULL) || (current->next == NULL && strcmp(current->key, key) > 0)){
      tempNode->next = current;
      *dict = tempNode;
    } else {
      while (current != NULL){
        if ((current->next == NULL) || (strcmp(current->next->key, key) > 0)){
          /*insert new node after current*/
          tempNode->next = current->next;
          current->next = tempNode;
          break;
        }
        current = current->next;
      }
    }
  } else {
    /*is in dictionary*/
    while (strcmp(current->key, keyCopy) != 0){
      current = current->next;
    }
    current->value = value;
  }
}
