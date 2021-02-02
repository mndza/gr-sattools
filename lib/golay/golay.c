/* -*- c++ -*- */
/* 
 * Copyright 2018 Diego Hurtado de Mendoza.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include <stdint.h>
#include <stdbool.h>

/* Golay generator matrix is G = (B | I), we only store the first half, B */
static const uint16_t golay_code[] = {
    0x8ED, 0x1DB, 0x3B5, 0x769,
    0xED1, 0xDA3, 0xB47, 0x68F,
    0xD1D, 0xA3B, 0x477, 0xFFE,
};

/* Strictly speaking, we do not encode a word because we only return
the first half, it is up to the user to join both parts */
uint16_t golay_encode(uint16_t word) {
	uint16_t 	golay = 0;

	for (int i = 0; i < 12; i++) {
		if (word & (0x800 >> i)) {
            golay ^= golay_code[i];
		}
	}

	return golay;
}

int32_t golay_decode(uint32_t word) {
	uint16_t 	syndrome = 0, syndrome2 = 0;
	uint8_t 	weight;
	uint32_t 	errorVector;

	// Notes about the following comments:
	// ===================================
	// Weight() function returns hamming weight of some vector
	// u = v + w, where u is the error vector, w is the received word and
	// v is the nearest codeword
	// b[i] is the i-th row of the matrix B (array golayCode) [G = B | I]

	word &= 0xFFFFFF;

	// Step 1. Compute the syndrome s = w·H'
	syndrome = (word >> 12) ^ golay_encode(word);

	// Step 2. If Weight(s) <= 3 then u = [s, 0]
	weight = __builtin_popcount(syndrome);
	if (weight <= 3) {
		errorVector = syndrome << 12;
		goto word_found;
	} 

	// Step 3. If Weight(s + b[i]) <= 2, then u = [s + b[i], 0x800 >> i]
	for (int i = 0; i < 12; i++) {
		weight = __builtin_popcount(syndrome ^ golay_code[i]);
		if (weight <= 2) {
			errorVector = (syndrome ^ golay_code[i]) << 12 | 0x800 >> i;
			goto word_found;
		}
	}

	// Step 4. Compute the second syndrome s·B
	syndrome2 = golay_encode(syndrome);
	weight = __builtin_popcount(syndrome2);

	//Step 5. If Weight(s·B) <= 3, then u = [0, s·B]
	if (weight <= 3) {
		errorVector = syndrome2;
		goto word_found;
	}

	// Step 6. If Weight(s·B + b[i]) <= 2, then u = [0x800 >> i, s·B + b[i]]
	for (int i = 0; i < 12; i++) {
		weight = __builtin_popcount(syndrome2 ^ golay_code[i]);
		if (weight <= 2) {
			errorVector = 0x800000 >> i | (syndrome2 ^ golay_code[i]);
			goto word_found;
		}
	}

	// Step 7. If u is not yet determined, then decoding is incorrect
	return -1;

	word_found:
	return word ^ errorVector;
}
