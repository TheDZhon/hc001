/**
 ** Work cycle interface.
 **/

#ifndef WORK_CYCLE_H_
#define WORK_CYCLE_H_

#include "definitions.h"

void wcycle_init ();
void wcycle_send (const char * str);
int wcycle_pwm_ctl (PWM_CTL_T);

#endif /* WORK_CYCLE_H_ */
