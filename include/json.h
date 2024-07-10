//
// Created by 666 on 10.07.2024.
//

#ifndef JSON_H
#define JSON_H

#include <string.h>
#include "stringmap.h"

#define MAX_STRING_LENGTH 1024

// PUBLIC API
StringMap* JSONDecode(const char* data);
char* JSONEncode(const StringMap* map, size_t limit);

/**
 *
 * @param buffer Buffer to write
 * @param offset Offset to write
 * @param map Map with data from JSON
 * @param indent Current indent level
 * @param limit Limit of buffer characters
 */
void JSONWriteStringMap(char* buffer, size_t* offset, const StringMap* map, size_t indent, size_t limit);

#endif //JSON_H
