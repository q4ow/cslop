#include "../include/archive.h"
#include "../include/format.h"
#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAGIC_BYTES_MAX 8

static const uint8_t ZIP_MAGIC[] = {0x50, 0x4B, 0x03, 0x04};
static const uint8_t GZIP_MAGIC[] = {0x1F, 0x8B};
static const uint8_t BZIP2_MAGIC[] = {0x42, 0x5A, 0x68};
static const uint8_t XZ_MAGIC[] = {0xFD, 0x37, 0x7A, 0x58, 0x5A, 0x00};
static const uint8_t _7Z_MAGIC[] = {0x37, 0x7A, 0xBC, 0xAF, 0x27, 0x1C};
static const uint8_t RAR_MAGIC[] = {0x52, 0x61, 0x72, 0x21};

archive_type_t detect_archive_type(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        print_error("Failed to open file");
        return ARCHIVE_TYPE_UNKNOWN;
    }

    uint8_t magic[MAGIC_BYTES_MAX];
    size_t read = fread(magic, 1, MAGIC_BYTES_MAX, f);
    fclose(f);

    if (read >= 4 && memcmp(magic, ZIP_MAGIC, 4) == 0)
        return ARCHIVE_TYPE_ZIP;
    if (read >= 2 && memcmp(magic, GZIP_MAGIC, 2) == 0)
        return ARCHIVE_TYPE_GZIP;
    if (read >= 3 && memcmp(magic, BZIP2_MAGIC, 3) == 0)
        return ARCHIVE_TYPE_BZIP2;
    if (read >= 6 && memcmp(magic, XZ_MAGIC, 6) == 0)
        return ARCHIVE_TYPE_XZ;
    if (read >= 6 && memcmp(magic, _7Z_MAGIC, 6) == 0)
        return ARCHIVE_TYPE_7Z;
    if (read >= 4 && memcmp(magic, RAR_MAGIC, 4) == 0)
        return ARCHIVE_TYPE_RAR;

    const char *ext = strrchr(filename, '.');
    if (ext && (strcmp(ext, ".tar") == 0))
        return ARCHIVE_TYPE_TAR;

    return ARCHIVE_TYPE_UNKNOWN;
}

const char *get_archive_type_name(archive_type_t type) {
    switch (type) {
    case ARCHIVE_TYPE_ZIP:
        return "ZIP";
    case ARCHIVE_TYPE_TAR:
        return "TAR";
    case ARCHIVE_TYPE_GZIP:
        return "GZIP";
    case ARCHIVE_TYPE_BZIP2:
        return "BZIP2";
    case ARCHIVE_TYPE_XZ:
        return "XZ";
    case ARCHIVE_TYPE_7Z:
        return "7Z";
    case ARCHIVE_TYPE_RAR:
        return "RAR";
    default:
        return "Unknown";
    }
}

static char *strip_archive_extensions(const char *filename) {
    char *base = basename((char *)filename);
    char *name = strdup(base);

    const char *compound_exts[] = {".tar.gz", ".tar.bz2", ".tar.xz", ".tgz",
                                   ".tbz2",   ".txz",     NULL};

    for (const char **ext = compound_exts; *ext; ext++) {
        char *pos = strcasestr(name, *ext);
        if (pos) {
            *pos = '\0';
            return name;
        }
    }

    char *start_ext = NULL;
    char *dot = name;
    while ((dot = strchr(dot, '.'))) {
        if (dot > name &&
            ((dot[-1] >= '0' && dot[-1] <= '9') ||
             (dot > name + 1 &&
              ((dot[-1] == 'v' && dot[-2] == '-') ||
               (isdigit(dot[-1]) && dot[-2] == 'v' && dot[-3] == '-'))))) {
            dot++;
            continue;
        }
        start_ext = dot;
        break;
    }

    if (start_ext)
        *start_ext = '\0';
    return name;
}

static char *create_extraction_dir(const char *archive_path,
                                   const struct extract_options *options) {
    char *dir;
    if (options->destination) {
        dir = strdup(options->destination);
    } else if (options->create_subdirectory) {
        dir = strip_archive_extensions(archive_path);
    } else {
        dir = strdup(".");
    }

    if (options->create_subdirectory) {
        struct stat st = {0};
        if (stat(dir, &st) == -1) {
            if (mkdir(dir, 0755) == -1) {
                print_error("Failed to create extraction directory");
                free(dir);
                return NULL;
            }
        }
    }

    return dir;
}

int extract_archive(const char *filename,
                    const struct extract_options *options) {
    archive_type_t type = detect_archive_type(filename);
    if (type == ARCHIVE_TYPE_UNKNOWN) {
        print_error("Unknown archive type");
        return -1;
    }

    char *extract_dir = create_extraction_dir(filename, options);
    if (!extract_dir)
        return -1;

    char command[1024];
    int success = 0;

    switch (type) {
    case ARCHIVE_TYPE_ZIP:
        snprintf(command, sizeof(command), "unzip %s %s -d %s",
                 options->overwrite ? "-o" : "-n", filename, extract_dir);
        break;

    case ARCHIVE_TYPE_TAR:
        snprintf(command, sizeof(command), "tar %s -xf %s -C %s",
                 options->preserve_permissions ? "" : "--no-same-permissions",
                 filename, extract_dir);
        break;

    case ARCHIVE_TYPE_GZIP:
        if (strstr(filename, ".tar.gz") || strstr(filename, ".tgz")) {
            snprintf(command, sizeof(command),
                     "mkdir -p %s && tar %s -xzf %s -C %s", extract_dir,
                     options->preserve_permissions ? ""
                                                   : "--no-same-permissions",
                     filename, extract_dir);
        } else {
            char *out_name = strip_archive_extensions(filename);
            snprintf(command, sizeof(command), "gunzip -c %s > %s/%s", filename,
                     extract_dir, out_name);
            free(out_name);
        }
        break;

    case ARCHIVE_TYPE_BZIP2:
        if (strstr(filename, ".tar.bz2") || strstr(filename, ".tbz2")) {
            snprintf(command, sizeof(command), "tar %s -xjf %s -C %s",
                     options->preserve_permissions ? ""
                                                   : "--no-same-permissions",
                     filename, extract_dir);
        } else {
            snprintf(command, sizeof(command), "bunzip2 -c %s > %s/%s",
                     filename, extract_dir, basename((char *)filename));
        }
        break;

    case ARCHIVE_TYPE_XZ:
        if (strstr(filename, ".tar.xz")) {
            snprintf(command, sizeof(command), "tar %s -xJf %s -C %s",
                     options->preserve_permissions ? ""
                                                   : "--no-same-permissions",
                     filename, extract_dir);
        } else {
            snprintf(command, sizeof(command), "unxz -c %s > %s/%s", filename,
                     extract_dir, basename((char *)filename));
        }
        break;

    case ARCHIVE_TYPE_7Z:
        snprintf(command, sizeof(command), "7z x %s -o%s", filename,
                 extract_dir);
        break;

    case ARCHIVE_TYPE_RAR:
        snprintf(command, sizeof(command), "unrar x %s %s", filename,
                 extract_dir);
        break;

    default:
        print_error("Unsupported archive type");
        free(extract_dir);
        return -1;
    }

    print_info("Archive type", get_archive_type_name(type));
    print_info("Extracting to", extract_dir);

    if (system(command) == 0) {
        print_success("Extraction completed successfully");
        success = 1;
    } else {
        print_error("Extraction failed");
        success = 0;
    }

    free(extract_dir);
    return success ? 0 : -1;
}
