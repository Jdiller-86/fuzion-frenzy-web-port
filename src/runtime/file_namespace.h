#ifndef FF_FILE_NAMESPACE_H
#define FF_FILE_NAMESPACE_H
#include "object_namespace.h"
#include "local_volume.h"

/* Bind the admitted guest volume and its file references, never host paths. */
uint32_t ff_file_namespace_bind(ff_object_namespace *, ff_local_volume *);
uint32_t ff_file_namespace_open(const ff_object_namespace *, const char *, uint32_t bytes,
                               uint32_t access, uint32_t share, uint32_t options, uint32_t *handle);
#endif
