
#include "NATestEnvironment.h"

typedef struct NATestListItem NATestListItem;
struct NATestListItem{
  NATestListItem* prev;
  NATestListItem* next;
  NATestUTF8Char* string;
};



NATestListItem* naAllocateTestListItem(const NATestUTF8Char* string){
  NATestListItem* item = (NATestListItem*)malloc(sizeof(NATestListItem));
  item->prev = item;
  item->next = item;
  if(string){
    size_t len = strlen(string);
    item->string = malloc(len + 1);
    item->string[len] = '\0';
    memcpy(item->string, string, len);
  }else{
    item->string = NATEST_NULL;
  }
  return item;
}

void naDeallocateTestListItem(NATestListItem* item){
  item->prev->next = item->next;
  item->next->prev = item->prev;
  free(item->string);
  free(item);
}

NATestBool naIsTestListEmpty(NATestListItem* item){
  return item->next == item;
}

void naAddTestListBefore(NATestListItem* thisItem, NATestListItem* item){
  item->prev = thisItem->prev;
  item->next = thisItem;
  thisItem->prev->next = item;
  thisItem->prev = item;
}



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
