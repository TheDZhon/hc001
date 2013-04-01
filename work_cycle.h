/**
 ** Work cycle interface.
 **/

#ifndef WORK_CYCLE_H_
#define WORK_CYCLE_H_

#include "definitions.h"

void wcycle_init ();
void wcycle_send (const char * str);

int wcycle_dht_ctl (char);
int wcycle_pwm_ctl (char);

#endif /* WORK_CYCLE_H_ */
