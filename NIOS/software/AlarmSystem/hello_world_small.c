#include <stdio.h>
#include <stdlib.h>
#include <sys/alt_driver.h>
#include <sys/alt_irq.h>
#include <sys/alt_stdio.h>

#include "alt_types.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_timer_regs.h"
#include "system.h"

// Global variables
volatile int current_seconds = 0;  // Timer interrupt flag
int current_hours = 0, current_minutes = 0;
int alarm_hours = 0, alarm_minutes = 33;  // Default alarm time
volatile unsigned short *key0 = (unsigned short *)PIO_KEY_0_BASE;
volatile unsigned short *key1 = (unsigned short *)PIO_KEY_1_BASE;
int ammount_pressed_key0 = 0;
int ammount_pressed_key1 = 0;
int last_state_key0 = 1;
int last_state_key1 = 1;
int active_alarm = 0;
int switchState = 0b00;

int decoder(int num) {
  int binarySegments = 0b1111111;

  if (num == 0) {
    binarySegments = 0b1000000;
  } else if (num == 1) {
    binarySegments = 0b1111001;
  } else if (num == 2) {
    binarySegments = 0b0100100;
  } else if (num == 3) {
    binarySegments = 0b0110000;
  } else if (num == 4) {
    binarySegments = 0b0011001;
  } else if (num == 5) {
    binarySegments = 0b0010010;
  } else if (num == 6) {
    binarySegments = 0b0000010;
  } else if (num == 7) {
    binarySegments = 0b1111000;
  } else if (num == 8) {
    binarySegments = 0b0000000;
  } else if (num == 9) {
    binarySegments = 0b0010000;
  } else {
    binarySegments = 0b0000001;
  }

  return binarySegments;
};

// Update the current time every second
void update_time(int *minutes, int *hours) {
  if (current_seconds >= 0) {
    current_seconds = 0;
    (*minutes)++;
    if (*minutes >= 60) {
      *minutes = 0;
      (*hours)++;
      if (*hours >= 24) *hours = 0;
    }
  }
}

void update_leds_and_buzzer(int *minutes, int *hours) {
  if (active_alarm % 2 != 0) {
    IOWR_ALTERA_AVALON_PIO_DATA(LEDS_MINUTES_MS_BASE, decoder(11));
    IOWR_ALTERA_AVALON_PIO_DATA(LEDS_MINUTES_LS_BASE, decoder(11));

    IOWR_ALTERA_AVALON_PIO_DATA(LEDS_HOURS_MS_BASE, decoder(11));
    IOWR_ALTERA_AVALON_PIO_DATA(LEDS_HOURS_LS_BASE, decoder(11));

    IOWR_ALTERA_AVALON_PIO_DATA(PIO_BUZZER_BASE, 1);

  } else {
    IOWR_ALTERA_AVALON_PIO_DATA(LEDS_MINUTES_MS_BASE, decoder((*minutes) / 10));
    IOWR_ALTERA_AVALON_PIO_DATA(LEDS_MINUTES_LS_BASE, decoder((*minutes) % 10));

    IOWR_ALTERA_AVALON_PIO_DATA(LEDS_HOURS_MS_BASE, decoder((*hours) / 10));
    IOWR_ALTERA_AVALON_PIO_DATA(LEDS_HOURS_LS_BASE, decoder((*hours) % 10));

    IOWR_ALTERA_AVALON_PIO_DATA(PIO_BUZZER_BASE, 0);
  }
  if (active_alarm != 0) {
    active_alarm--;
  }
}

// Alarm check
void check_alarm(int current_minutes, int current_hours, int alarm_minutes,
                 int alarm_hours) {
  if (current_hours == alarm_hours && current_minutes == alarm_minutes) {
    active_alarm = 30;
  }
}

void set_clock(int *hours, int *minutes) {
  //
}

// Timer ISR
void timer_isr(void *context, alt_u32 id) {
  switchState = IORD_ALTERA_AVALON_PIO_DATA(PIO_SWITCHES_BASE);

  switch (switchState) {
    case 0b00:
      // Alarma Encendida
      update_time(&current_minutes,
                  &current_hours);  // Actualizar la hora actual
      check_alarm(current_minutes, current_hours, alarm_minutes, alarm_hours);
      update_leds_and_buzzer(
          &current_minutes,
          &current_hours);  // Actualizar el valor de los LEDs
                            // active_alarm = ammount_pressed_key0 > 0 ||
                            // ammount_pressed_key1 > 0
      //                    ? active_alarm
      //                    : 0;
      break;
    case 0b01:
      current_minutes += ammount_pressed_key0;
      current_minutes = current_minutes >= 60 ? 0 : current_minutes;
      current_hours += ammount_pressed_key1;
      current_hours = current_hours >= 24 ? 0 : current_hours;
      update_leds_and_buzzer(
          &current_minutes, &current_hours);  // Actualizar el valor de los LEDs
      break;

    case 0b10:
      alarm_minutes += ammount_pressed_key0;
      alarm_minutes = alarm_minutes >= 60 ? 0 : alarm_minutes;
      alarm_hours += ammount_pressed_key1;
      alarm_hours = alarm_hours >= 24 ? 0 : alarm_hours;
      update_leds_and_buzzer(&alarm_minutes,
                             &alarm_hours);  // Actualizar el valor de los LEDs
      break;

    case 0b11:
      // Alarma Apagada
      update_time(&current_minutes,
                  &current_hours);  // Actualizar la hora actual
      update_leds_and_buzzer(
          &current_minutes, &current_hours);  // Actualizar el valor de los LEDs
      break;

    default:
      // Error: no se ha pulsado ninguna tecla
      break;
  }
  char str[12];
  if (ammount_pressed_key0 != 0) {
    alt_putstr("key0 =  \n");
    itoa(ammount_pressed_key0, str, 10);
    alt_putstr(str);
    alt_putstr("\n");
    ammount_pressed_key0 = 0;
  }
  if (ammount_pressed_key1 != 0) {
    alt_putstr("key1 = \n");
    itoa(ammount_pressed_key1, str, 10);
    alt_putstr(str);
    alt_putstr("\n");
    ammount_pressed_key1 = 0;
  }

  IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);  // Clear the interrupt
  current_seconds += 1;  // Flag that a second has passed

  // Check if it is time to activate the alarm
}

void init_timer() {
  unsigned int period_value =
      TIMER_FREQ;  // Calculate the period value for 1 second

  // Set timer period
  IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_BASE, (alt_u16)period_value);
  IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_BASE, (alt_u16)(period_value >> 16));

  // Enable timer with continuous mode and interrupt enabled
  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE,
                                   ALTERA_AVALON_TIMER_CONTROL_ITO_MSK |
                                       ALTERA_AVALON_TIMER_CONTROL_CONT_MSK |
                                       ALTERA_AVALON_TIMER_CONTROL_START_MSK);

  // Register ISR
  alt_ic_isr_register(TIMER_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_IRQ, timer_isr,
                      NULL, NULL);
}

int main() {
  IOWR_ALTERA_AVALON_PIO_DATA(LEDS_MINUTES_LS_BASE, decoder(0));
  IOWR_ALTERA_AVALON_PIO_DATA(LEDS_MINUTES_MS_BASE, decoder(0));
  IOWR_ALTERA_AVALON_PIO_DATA(LEDS_HOURS_LS_BASE, decoder(0));
  IOWR_ALTERA_AVALON_PIO_DATA(LEDS_HOURS_MS_BASE, decoder(0));
  IOWR_ALTERA_AVALON_PIO_DATA(PIO_BUZZER_BASE, 0);

  init_timer();  // Initialize the timer
  int state_key0, state_key1;

  while (1) {
    // Read current state of each button
    state_key0 = IORD_ALTERA_AVALON_PIO_DATA(key0);
    state_key1 = IORD_ALTERA_AVALON_PIO_DATA(key1);

    // Check for button 0 release
    if (last_state_key0 == 0 && state_key0 == 1) {
      // Button 0 was released
      ammount_pressed_key0 += 1;
    }

    // Check for button 1 release
    if (last_state_key1 == 0 && state_key1 == 1) {
      // Button 1 was released
      ammount_pressed_key1 += 1;
    }

    // Update last states
    last_state_key0 = state_key0;
    last_state_key1 = state_key1;
  }

  return 0;
}
