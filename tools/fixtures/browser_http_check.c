/* Source-owned boundary check; all arguments contain synthetic content only. */
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "qemu/typedefs.h"
#include "qapi/error.h"
#include "qemu/http.h"

struct Error { int unused; };
static Error marker;
static unsigned errors, callbacks;
static char message[128];

/* Replace only the host's error allocator in this isolated link check. */
void error_setg_internal(Error **errp, const char *src, int line,
                         const char *func, const char *fmt, ...)
{
    (void)src;
    (void)line;
    (void)func;
    va_list args;
    va_start(args, fmt);
    vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);
    errors++;
    if (errp) {
        *errp = &marker;
    }
}

static int progress(http_progress_cb_info *info)
{
    (void)info;
    callbacks++;
    return 0;
}

int main(void)
{
    Error *error = NULL;
    unsigned char data[] = "unchanged";
    GByteArray body = { data, sizeof(data) };
    http_progress_cb_info info = { .progress = progress, .dlnow = 7 };
    if (ensure_libcurl_initialized(&error) || error != &marker) return 1;
    error = NULL;
    if (http_get("https://example.invalid/synthetic", &body, &info, &error) != -1 ||
        error != &marker || strcmp((const char *)data, "unchanged") ||
        body.len != sizeof(data) || info.dlnow != 7 || callbacks) return 2;
    error = NULL;
    if (http_post_json("https://example.invalid/synthetic", "synthetic", &error) != -1 ||
        error != &marker) return 3;
    if (http_get(NULL, NULL, NULL, NULL) != -1 ||
        http_post_json(NULL, NULL, NULL) != -1 ||
        ensure_libcurl_initialized(NULL)) return 4;
    if (errors != 6 || strcmp(message,
        "Native HTTP is disabled in the browser host profile")) return 5;
    puts("Browser host HTTP: failure, error, unchanged-buffer and no-callback checks passed");
    return 0;
}
