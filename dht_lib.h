#ifndef DHT_LIB_H_
#define DHT_LIB_H_

#include <stdint.h>

#include "definitions.h"

#include "work_cycle.h"

class DHT {
public:
	DHT ();
	~DHT ();

	inline void handle_timer (uint16_t v) {
		signal_[signal_indx_++] = v;
	}

	int decode ();

	void humidity (uint16_t * ipart, uint16_t * dpart) const;
	void temperature (uint16_t * ipart, uint16_t * dpart) const;
private:
	unsigned int signal_[DHT_SIG_BUF_SZ];
	unsigned short data_[DHT_DATA_BUF_SZ];

	unsigned signal_indx_;
};

#endif /* DHT_LIB_H_ */
