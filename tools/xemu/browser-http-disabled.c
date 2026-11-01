/* Browser host profile: native HTTP services are deliberately unavailable.
 * This host adapter does not emulate or stub any Xbox kernel/device behavior.
 */
#include <stdbool.h>
#include <glib.h>
#include "qemu/typedefs.h"
#include "qapi/error.h"
#include "qemu/http.h"

static void unavailable(Error **errp)
{
    error_setg(errp, "Native HTTP is disabled in the browser host profile");
}

bool ensure_libcurl_initialized(Error **errp)
{
    unavailable(errp);
    return false;
}

int http_get(const char *url, GByteArray *response_body,
             http_progress_cb_info *progress_info, Error **errp)
{
    (void)url;
    (void)response_body;
    (void)progress_info;
    unavailable(errp);
    return -1;
}

int http_post_json(const char *url, const char *json_data, Error **errp)
{
    (void)url;
    (void)json_data;
    unavailable(errp);
    return -1;
}
