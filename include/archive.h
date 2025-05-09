#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <stdint.h>

typedef enum {
  ARCHIVE_TYPE_UNKNOWN,
  ARCHIVE_TYPE_ZIP,
  ARCHIVE_TYPE_TAR,
  ARCHIVE_TYPE_GZIP,
  ARCHIVE_TYPE_BZIP2,
  ARCHIVE_TYPE_XZ,
  ARCHIVE_TYPE_7Z,
  ARCHIVE_TYPE_RAR
} archive_type_t;

struct extract_options {
  const char *destination;
  int create_subdirectory;
  int preserve_permissions;
  int overwrite;
};

archive_type_t detect_archive_type(const char *filename);
const char *get_archive_type_name(archive_type_t type);
int extract_archive(const char *filename,
                    const struct extract_options *options);

#endif
