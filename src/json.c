//
// Created by nathan on 1/3/25.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SEND_JSON(json) do { \
    write(client_socket, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n", 51); \
    write(client_socket, (json), strlen(json)); \
} while (0)

char* extract_json_value(const char* json, const char* key, char* buffer, size_t buffer_size) {
    char search_key[128];
    snprintf(search_key, sizeof(search_key), "\"%s\"", key);

    const char* key_pos = strstr(json, search_key);
    if (!key_pos) return NULL;

    const char* value_start = strchr(key_pos, ':');
    if (!value_start) return NULL;

    value_start++; // Skip ':'
    while (isspace(*value_start)) value_start++; // Skip whitespace

    if (*value_start == '\"') {
        // String value
        value_start++;
        const char* value_end = strchr(value_start, '\"');
        if (!value_end) return NULL;
        size_t length = value_end - value_start;
        if (length >= buffer_size) length = buffer_size - 1;
        strncpy(buffer, value_start, length);
        buffer[length] = '\0';
    } else {
        // Numeric value
        const char* value_end = strpbrk(value_start, ",}");
        if (!value_end) value_end = json + strlen(json);
        size_t length = value_end - value_start;
        if (length >= buffer_size) length = buffer_size - 1;
        strncpy(buffer, value_start, length);
        buffer[length] = '\0';
    }

    return buffer;
}
