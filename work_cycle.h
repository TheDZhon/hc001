/**
 ** Work cycle interface.
 **/

#ifndef WORK_CYCLE_H_
#define WORK_CYCLE_H_

#include "definitions.h"

void wcycle_init ();

int wcycle_send (char*);
int wcycle_read (char*);
int wcycle_pwm_ctl (PWM_CTL_T);

#endif /* WORK_CYCLE_H_ */
