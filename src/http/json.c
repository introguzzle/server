//
// Created by 666 on 10.07.2024.
//

#include "json.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "strings.h"
#include "../third-party/cJSON.h"
#include "stringmap.h"
#include "vector.h"

#define MAX_STRING_LENGTH 1024
static const char* TAB = "  ";

static void JSONWriteIndent(char* buffer, size_t* offset, const size_t indent, const size_t limit) {
    for (size_t i = 0; i < indent; i++) {
        *offset += snprintf(buffer + *offset, limit - *offset, "%s", TAB);
    }
}

// Функция для записи вектора в буфер
static void JSONWriteVector(char* buffer, size_t* offset, const Vector* vector, const size_t indent, const size_t limit) {
    *offset += snprintf(buffer + *offset, limit - *offset, "[\n");

    for (size_t i = 0; i < vector->size; i++) {
        JSONWriteIndent(buffer, offset, indent + 1, limit);
        void* item = vector->items[i];

        if (IsVector(item)) {
            JSONWriteVector(buffer, offset, item, indent + 1, limit);
        } else if (IsStringMap(item)) {
            JSONWriteStringMap(buffer, offset, item, indent + 1, limit);
        } else {
            *offset += snprintf(buffer + *offset, limit - *offset, "\"%s\"", item == NULL ? "null" : (const char*) item);
        }

        if (i < vector->size - 1) {
            *offset += snprintf(buffer + *offset, limit - *offset, ",");
        }

        *offset += snprintf(buffer + *offset, limit - *offset, "\n");
    }

    JSONWriteIndent(buffer, offset, indent, limit);
    *offset += snprintf(buffer + *offset, limit - *offset, "]");
}

void JSONWriteStringMap(char* buffer, size_t* offset, const StringMap* map, const size_t indent, const size_t limit) {
    *offset += snprintf(buffer + *offset, limit - *offset, "{\n");

    for (size_t i = 0; i < map->size; ++i) {
        const char* key = map->items[i].key;
        void* value = map->items[i].value;

        JSONWriteIndent(buffer, offset, indent + 1, limit);
        *offset += snprintf(buffer + *offset, limit - *offset, "\"%s\": ", key);

        if (IsStringMap(value)) {
            JSONWriteStringMap(buffer, offset, value, indent + 1, limit);
        } else if (IsVector(value)) {
            JSONWriteVector(buffer, offset, value, indent + 1, limit);
        } else {
            *offset += snprintf(buffer + *offset, limit - *offset, "\"%s\"", value == NULL ? "null" : (const char*) value);
        }

        if (map->size - 1 != i) {
            *offset += snprintf(buffer + *offset, limit - *offset, ",");
        }

        *offset += snprintf(buffer + *offset, limit - *offset, "\n");
    }

    JSONWriteIndent(buffer, offset, indent, limit);
    *offset += snprintf(buffer + *offset, limit - *offset, "}");
}

static char* JSONDoubleToString(const double value) {
    char* str = NewString(64 / 8);
    if (str != NULL) {
        snprintf(str, MAX_STRING_LENGTH, "%f", value);
    }

    return str;
}

static char* JSONIntegerToString(const long long value) {
    char* str = NewString(64 / 8);
    lltoa(value, str, 10);
    return str;
}

static void JSONParseObject(const cJSON* jsonObject, StringMap* result) {
    cJSON* item = NULL;
    cJSON_ArrayForEach(item, jsonObject) {
        const char* key = item->string;

        if (cJSON_IsString(item)) {
            char* value = strdup(item->valuestring);
            StringMapPut(result, key, value);

        } else if (cJSON_IsNull(item)) {
            StringMapPut(result, key, NULL);

        } else if (cJSON_IsBool(item)) {
            char* value = NewString(MAX_STRING_LENGTH);
            snprintf(value, MAX_STRING_LENGTH, "%s", item->valueint ? "true" : "false");
            StringMapPut(result, key, value);

        } else if (cJSON_IsNumber(item)) {
            double _;
            const double remainder = modf(item->valuedouble, &_);

            char* value;
            if (fabs(remainder) < DBL_EPSILON * 1e10) {
                value = JSONIntegerToString(item->valueint);
            } else {
                value = JSONDoubleToString(item->valuedouble);
            }

            StringMapPut(result, key, value);

        } else if (cJSON_IsArray(item)) {
            Vector* vector = NewVector();

            cJSON* element = NULL;
            cJSON_ArrayForEach(element, item) {
                if (cJSON_IsString(element)) {
                    char* value = strdup(element->valuestring);
                    VectorPush(vector, value);

                } else if (cJSON_IsNumber(element)) {
                    double _;
                    const double remainder = modf(element->valuedouble, &_);

                    char* value;
                    if (fabs(remainder) < DBL_EPSILON * 1e10) {
                        value = JSONIntegerToString(element->valueint);
                    } else {
                        value = JSONDoubleToString(element->valuedouble);
                    }

                    VectorPush(vector, value);

                } else if (cJSON_IsBool(element)) {
                    char* value = NewString(MAX_STRING_LENGTH);
                    snprintf(value, MAX_STRING_LENGTH, "%s", element->valueint ? "true" : "false");
                    VectorPush(vector, value);

                } else if (cJSON_IsNull(element)) {
                    VectorPush(vector, NULL);

                } else if (cJSON_IsArray(element) || cJSON_IsObject(element)) {
                    StringMap* nested = NewStringMap();
                    JSONParseObject(element, nested);
                    VectorPush(vector, nested);
                }
            }

            StringMapPut(result, key, vector);

        } else if (cJSON_IsObject(item)) {
            StringMap* nestedMap = NewStringMap();
            JSONParseObject(item, nestedMap);
            StringMapPut(result, key, nestedMap);
        }
    }
}

StringMap* JSONParse(const char* jsonString) {
    cJSON* json = cJSON_Parse(jsonString);
    StringMap* result = NewStringMap();

    if (json == NULL) {
        return result;
    }

    if (cJSON_IsObject(json)) {
        JSONParseObject(json, result);
    }

    cJSON_Delete(json);
    return result;
}

StringMap* JSONDecode(const char* data) {
    return JSONParse(data);
}

char* JSONEncode(const StringMap* map, const size_t limit) {
    char* encoded = NewString(limit);
    size_t offset = 0;
    JSONWriteStringMap(encoded, &offset, map, 0, limit);
    return encoded;
}
