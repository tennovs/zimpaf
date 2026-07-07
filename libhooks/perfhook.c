/*
 * Copyright © 2026 Tennov Simanjuntak, The University of Texas at Arlington
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

 #include "../include/perfhook.h"
 #include "zend_string.h"
 #include <stdlib.h>

#if PHP_VERSION_ID >= 80400
    
    typedef struct {
        void* (*malloc)(size_t ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC);
        void  (*free)(void* ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC);
        void* (*realloc)(void*, size_t ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC);
    } zimpaf_original_handlers_t;
#else
    typedef struct {
        void* (*malloc)(size_t);
        void  (*free)(void*);
        void* (*realloc)(void*, size_t);
    } zimpaf_original_handlers_t;
#endif


static zimpaf_original_handlers_t original_handlers;
static bool zimpaf_in_hook = false;

#if PHP_VERSION_ID >= 80400
    void *zimpaf_malloc(size_t size ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC){
        printf("zimpaf_malloc called with size: %zu\n", size);
        if (original_handlers.malloc) {
            // return original_handlers.malloc(size ZEND_FILE_LINE_RELAY_CC ZEND_FILE_LINE_ORIG_RELAY_CC);
            

            void *p = original_handlers.malloc(
                size
                ZEND_FILE_LINE_RELAY_CC
                ZEND_FILE_LINE_ORIG_RELAY_CC
            );

            printf("malloc(%zu) -> %p\n", size, p);

            return p;

        }else {
            
        // return emalloc(size);
        return zend_mm_alloc(zend_mm_get_heap(), size ZEND_FILE_LINE_RELAY_CC ZEND_FILE_LINE_ORIG_RELAY_CC);
        }
    }

    void zimpaf_free(void *ptr ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC){
    if (zimpaf_in_hook) {
        // We are already inside the hook, just call the real system free/efree
        _efree(ptr ZEND_FILE_LINE_RELAY_CC ZEND_FILE_LINE_ORIG_RELAY_CC);
        return;
    }

    zimpaf_in_hook = true; // Enter guard
    printf("zimpaf_free called\n");

    if (original_handlers.free) {
        original_handlers.free(ptr ZEND_FILE_LINE_RELAY_CC ZEND_FILE_LINE_ORIG_RELAY_CC);
    } else {
        _efree(ptr ZEND_FILE_LINE_RELAY_CC ZEND_FILE_LINE_ORIG_RELAY_CC);
    }
    
    zimpaf_in_hook = false; // Exit guard
}

    void *zimpaf_realloc(void *ptr, size_t size ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC){
        printf("zimpaf_realloc called with size: %zu\n", size);
        if (original_handlers.realloc) {
            return original_handlers.realloc(ptr, size ZEND_FILE_LINE_RELAY_CC ZEND_FILE_LINE_ORIG_RELAY_CC);
        }else {
        return realloc(ptr, size);
        }
    }

#else
    void *zimpaf_malloc(size_t len){
        return malloc(len);
    }
    void zimpaf_free(void *ptr){
         free(ptr);
    }
    void *zimpaf_realloc(void *ptr, size_t len){
        return realloc(ptr, len);
    }
#endif

void hook_memory_management(){
    zend_mm_heap *heap = zend_mm_get_heap();
    original_handlers.malloc = NULL;
    original_handlers.free = NULL;
    original_handlers.realloc = NULL;

    if (is_zend_mm()){
        
            zend_mm_get_custom_handlers(heap, 
                                        &original_handlers.malloc, 
                                        &original_handlers.free, 
                                        &original_handlers.realloc);

            printf("heap=%p\n", (void *)heap);
            printf("malloc=%p free=%p realloc=%p\n",
            (void *)original_handlers.malloc,
            (void *)original_handlers.free,
            (void *)original_handlers.realloc);
     

            zend_mm_set_custom_handlers(
                heap,
                zimpaf_malloc, 
                zimpaf_free, 
                zimpaf_realloc);
        
    }
}