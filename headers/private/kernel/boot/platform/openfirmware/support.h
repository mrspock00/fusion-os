/*
 * Copyright 2006-2011, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *      Alexander von Gluck, kallisti5@unixzen.com
 */
#ifndef SUPPORT_H
#define SUPPORT_H


#include <OS.h>


bigtime_t system_time(void);
int32 of_address_cells(intptr_t package);
int32 of_size_cells(intptr_t package);

#endif
