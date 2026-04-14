#pragma once

#include "hal_cfg.h"

#include "utl_dbg.h"
#include "utl_crc16.h"
#include "utl_cbf.h"
#include "utl_io.h"

#if UTL_COBS_ENABLED == 1
#include "utl_cobs.h"
#endif

#if UTL_AT_ENABLED == 1
#include "utl_at.h"
#endif

#if UTL_KVED_ENABLED == 1
#include "kved.h"
#endif

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
