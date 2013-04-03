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

#ifndef DHT_LIB_H_
#define DHT_LIB_H_

#include <stdint.h>

#include "definitions.h"

#include "work_cycle.h"

/**
 ** Main DHT class.
 ** Currently supports only DHT22, but can be painless ported for DHT11 too.
 **/
class DHT {
public:
	/**
	 ** Default ctor.
	 **/
	DHT ();
	/**
	 ** Default dtor.
	 **/
	~DHT ();
	/**
	 ** Register timer update.
	 **
	 ** @param[in]   v   Duration of detected pulse from DHT
	 **/
	inline void handle_timer (uint16_t v) {	signal_[signal_indx_++] = v; }
	/**
	 ** Try capture values from measured timer updates.
	 **
	 ** @return   Operation status code
	 **/
	int decode ();
	/**
	 ** Fill humidity value.
	 **
	 ** @param[out]   ipart   Integral part
	 ** @param[out]   dpart   Fractional part
	 **/
	void humidity (uint16_t * ipart, uint16_t * dpart) const;
	/**
	 ** Fill temperature value.
	 **
	 ** @param[out]   ipart   Integral part
	 ** @param[out]   dpart   Fractional part
	 **/
	void temperature (uint16_t * ipart, uint16_t * dpart) const;
private:
	unsigned int signal_[DHT_SIG_BUF_SZ];
	unsigned short data_[DHT_DATA_BUF_SZ];

	unsigned signal_indx_;
};

#endif /* DHT_LIB_H_ */
