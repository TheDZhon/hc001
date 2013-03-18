#include "dht_lib.h"

uint16_t uword (uint8_t h, uint8_t l)
{
	uint16_t ret = h;
	ret <<= 8;
	ret += l;
	return ret;
}

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
	if (signal_indx_ != DHT_SIG_BUF_SZ) { return signal_indx_; }

	signal_indx_ = 0;

    for (unsigned j = 0; j < DHT_DATA_BUF_SZ; ++j) {
        data_[j] = 0;
        for (unsigned i = 0; i < 8; ++i) {
            int k = i + 2 + j * 8;
            if ((signal_[k] > DHT_ONE_MIN_DUR) && (signal_[k] < DHT_ONE_MAX_DUR)) {
                data_[j] |= (1 << (7 - i));
            }
        }
    }

    const unsigned short sum = (data_[0] + data_[1] + data_[2] + data_[3]) & 0xFF;

	return (sum == data_[4]) ? 0 : -1;
}

void DHT::humidity (uint16_t * ipart, uint16_t * dpart) const {
	decode_dht22 (uword (data_[0], data_[1]), ipart, dpart);
}

void DHT::temperature (uint16_t * ipart, uint16_t * dpart) const {
	decode_dht22 (uword (data_[2] & 0x7F, data_[3]), ipart, dpart);
}
