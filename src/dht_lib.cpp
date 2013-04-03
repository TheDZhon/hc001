/**
 ** DHT22 support library for MSP430.
 **
 ** Copyright (c) 2013, Eugene Mamin
 ** All rights reserved.
 ** Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are met:
 **
 ** 1. Redistributions of source code must retain the above copyright notice, this
 ** list of conditions and the following disclaimer.
 ** 2. Redistributions in binary form must reproduce the above copyright notice,
 ** this list of conditions and the following disclaimer in the documentation
 ** and/or other materials provided with the distribution.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 ** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 ** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ** ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 ** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 ** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 ** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ** The views and conclusions contained in the software and documentation are those
 ** of the authors and should not be interpreted as representing official policies,
 ** either expressed or implied, of the FreeBSD Project
 **/

#include "dht_lib.h"

/**
 ** Data array interpretation.
 **/
enum data_indexes_t {
	kHl = 0,
	kHh = 1,
	kTl = 2,
	kTh = 3,
	kCRC = 4
};

/**
 ** Helper function to combine high and low bytes into single 16-bit word.
 **/
uint16_t uword (uint8_t h, uint8_t l)
{
	uint16_t ret = h;
	ret <<= 8;
	ret += l;
	return ret;
}

/**
 ** Helper function to decode DHT22 values with fractional part without float division.
 **/
void decode_dht22 (uint16_t uw, uint16_t * ipart, uint16_t * dpart)
{
	*ipart = uw / 10;
	*dpart = uw % 10;
}

DHT::DHT():
	signal_(),
	data_(),
	signal_indx_(0)
{}

DHT::~DHT() {}

int DHT::decode ()
{
	if (signal_indx_ != DHT_SIG_BUF_SZ) {
		signal_indx_ = 0;
		return DHT_LINK_ERROR;
	}

	signal_indx_ = 0;

    for (unsigned j = 0; j < DHT_DATA_BUF_SZ; ++j) {
        data_[j] = 0;
        for (unsigned i = 0; i < 8; ++i) {
            int k = i + 2 + j * 8;
            if ((signal_[k] > DHT_ZERO_MAX_DUR) && (signal_[k] < DHT_ONE_MAX_DUR)) {
                data_[j] |= (1 << (7 - i));
            }
        }
    }

    // For control sum checking
    const unsigned short sum = (data_[0] + data_[1] + data_[2] + data_[3]) & 0xFF;

	return (sum == data_[kCRC]) ? DHT_SUCCESS_STATUS : DHT_CRC_ERROR;
}

void DHT::humidity (uint16_t * ipart, uint16_t * dpart) const {
	decode_dht22 (uword (data_[kHl], data_[kHh]), ipart, dpart);
}

void DHT::temperature (uint16_t * ipart, uint16_t * dpart) const {
	decode_dht22 (uword (data_[kTl] & 0x7F, data_[kTh]), ipart, dpart);
}
