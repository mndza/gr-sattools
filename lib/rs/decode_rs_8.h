/* Reed-Solomon decoder
 * Copyright 2002 Phil Karn, KA9Q
 * May be used under the terms of the GNU General Public License (GPL)
 */

#include <stdint.h>

int decode_rs_8(uint8_t *data, int *eras_pos, int no_eras);