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

#ifndef PERFORMANCE_HOOK_H
#define PERFORMANCE_HOOK_H

#include "zend.h"
#include "zend_alloc.h"
#include "php.h"

#endif

#if PHP_VERSION_ID >= 80400
    void *zimpaf_malloc(size_t ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC);
    void zimpaf_free(void *ptr ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC);
    void *zimpaf_realloc(void *ptr, size_t ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC);
#else
    void *zimpaf_malloc(size_t len);
    void zimpaf_free(void *ptr);
    void *zimpaf_realloc(void *ptr, size_t len);
#endif


void hook_memory_management();
