/*
 * dht_lib.h
 *
 *  Created on: 13.03.2013
 *      Author: dzhon
 */

#ifndef DHT_LIB_H_
#define DHT_LIB_H_

#include <stdint.h>

class DHT {
public:
	DHT ();
	~DHT ();

	inline void handle_timer (uint16_t v) { signal_[signal_indx_++] = v; }

	int decode ();

	void humidity (uint16_t * ipart, uint16_t * dpart) const;
	void temperature (uint16_t * ipart, uint16_t * dpart) const;
private:
	unsigned int signal_[42];
	unsigned short data_[5];

	unsigned signal_indx_;
};

#endif /* DHT_LIB_H_ */
