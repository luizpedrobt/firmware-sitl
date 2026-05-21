#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include "utl.h"

#define UTL_DBG_NUM_CHARS_PER_LINE 32

const uint8_t *utl_log_mod_name[] = {
#define X(MOD, INDEX) (uint8_t *)#MOD,
    XMACRO_DBG_MODULES
#undef X
};

static uint32_t utl_dbg_mods_activated = 0;

const char *utl_dbg_base_name_get(const char *full_path)
{
    const char *base_name = full_path;

    while (*full_path)
    {
        if (*full_path == '/' || *full_path == '\\')
            base_name = full_path + 1;

        full_path++;
    }

    return base_name;
}

const uint8_t *utl_dbg_mod_name_get(utl_dbg_modules_t mod_idx)
{
    return (const uint8_t *)&(utl_log_mod_name[mod_idx][12]);
}

void utl_dbg_mod_enable(utl_dbg_modules_t mod_idx)
{
    utl_dbg_mods_activated |= 1 << mod_idx;
}

void utl_dbg_mod_disable(utl_dbg_modules_t mod_idx)
{
    utl_dbg_mods_activated &= ~((uint32_t)(1 << mod_idx));
}

bool utl_dbg_mod_enabled(utl_dbg_modules_t mod_idx)
{
    return (utl_dbg_mods_activated & (1 << mod_idx)) > 0;
}

void utl_dbg_dump(char *stamp, uint8_t *data, size_t size)
{
    uint8_t *ptr = data;
    uint8_t ascii[UTL_DBG_NUM_CHARS_PER_LINE + 1];
    size_t ascii_pos = 0;
    char buf[128];
    int len;

    len = snprintf(buf, sizeof(buf), "%s", stamp);
    if (len > 0)
        write(STDOUT_FILENO, buf, len);

    for (size_t pos = 0; pos < size; pos++)
    {
        if (pos && (pos % UTL_DBG_NUM_CHARS_PER_LINE == 0))
        {
            ascii[ascii_pos] = '\0';
            len = snprintf(buf, sizeof(buf), " %s\n%s", (char *)ascii, stamp);
            if (len > 0)
                write(STDOUT_FILENO, buf, len);
            ascii_pos = 0;
        }

        if (pos % UTL_DBG_NUM_CHARS_PER_LINE == 0)
        {
            len = snprintf(buf, sizeof(buf), "%04X ", (unsigned int)pos);
            if (len > 0)
                write(STDOUT_FILENO, buf, len);
        }

        ascii[ascii_pos++] = isprint(*ptr) ? *ptr : '.';
        len = snprintf(buf, sizeof(buf), "%02X", *ptr++);
        if (len > 0)
            write(STDOUT_FILENO, buf, len);
    }
    ascii[ascii_pos] = '\0';
    len = snprintf(buf, sizeof(buf), " %s\n", (char *)ascii);
    if (len > 0)
        write(STDOUT_FILENO, buf, len);
}

void utl_dbg_init(void)
{
    utl_dbg_mod_enable(UTL_DBG_MOD_APP);
    // utl_dbg_mod_enable(UTL_DBG_MOD_GPIO);
}
