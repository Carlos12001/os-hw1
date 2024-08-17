#include "system.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_pio_regs.h"
#include "alt_types.h"
#include "sys/alt_irq.h"
#include <stdio.h>
#include <stdlib.h>

// Variables globales
volatile int timer_flag = 0; // Indicador de interrupción del temporizador
int current_hours = 0, current_minutes = 0;
int alarm_hours = 0, alarm_minutes = 5; // Hora de alarma predeterminada

int switchState = 0b00;

int decoder (int num) {
	int binarySegments = 0b1111111;

	if(num == 0){
		binarySegments = 0b1000000;
	}else if(num == 1){
		binarySegments = 0b1111001;
	}else if(num == 2){
		binarySegments = 0b0100100;
	}else if(num == 3){
		binarySegments = 0b0110000;
	}else if(num == 4){
		binarySegments = 0b0011001;
	}else if(num == 5){
		binarySegments = 0b0010010;
	}else if(num == 6){
		binarySegments = 0b0000010;
	}else if(num == 7){
		binarySegments = 0b1111000;
	}else if(num == 8){
		binarySegments = 0b0000000;
	}else if(num == 9){
		binarySegments = 0b0010000;
	}else{
		binarySegments = 0b0000001;
	}

	return binarySegments;
};


// ISR del temporizador
void timer_isr(void* context, alt_u32 id) {
	switchState = IORD_ALTERA_AVALON_PIO_DATA(PIO_SWITCHES_BASE);

	if (switchState == 0b01) {
		// Agregar aquí para configurar la hora del reloj
		//set_clock(&current_hours, &current_minutes);
	}
	if (switchState == 0b10) {
		// Agregar aquí para configurar la hora de la alarma
	}
	if (switchState == 0b11) {
		update_time(&current_hours, &current_minutes); // Actualiza la hora actual
	}


	if (current_minutes != 0 && current_minutes%10 == 0){
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_SECONDS_MS_BASE, decoder(current_minutes/10));
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_SECONDS_LS_BASE, decoder(0));
	} else {
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_SECONDS_LS_BASE, decoder(current_minutes%10));
	}

	if (current_hours != 0 && current_hours%10 == 0){
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_MINUTES_MS_BASE, decoder(current_hours/10));
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_MINUTES_LS_BASE, decoder(0));
	} else {
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_MINUTES_LS_BASE, decoder(current_hours%10));
	}

	// Para pruebas, imprime la hora actual
	/*char str[12];
	alt_printf("Hour: ");
	itoa(current_hours, str, 10);
	alt_printf(str);
	alt_printf(" and minutes: ");
	itoa(current_minutes, str, 10);
	alt_printf(str);
	alt_printf("\n");*/


    IOWR_ALTERA_AVALON_TIMER_STATUS(0x3020, 0); // Limpia la interrupción
    timer_flag = 1; // Marca que ha pasado un segundo


    // Comprueba si es hora de activar la alarma
    check_alarm(current_hours, current_minutes, alarm_hours, alarm_minutes);
}


// ISR del botón de horas
void button_hours_isr(void* context, alt_u32 id) {
    //debounce();
	alt_printf("Caca");

	current_hours++;
	if (current_hours >= 24) current_hours = 0;

    // Limpiar la interrupción del botón
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY_3_BASE, 0);
}


void set_clock(int* hours, int* minutes) {
    //
}

// Inicialización del temporizador
void init_timer() {
    // Configuración del temporizabdor para generar una interrupción cada segundo
	IOWR_ALTERA_AVALON_TIMER_PERIODL(0x3020, 33333333*2 & 0xFFFF); // Valor bajo del periodo para 1 segundo
    IOWR_ALTERA_AVALON_TIMER_PERIODH(0x3020, (33333333*2>>16)&0xFFFF); // Valor alto del periodo
    IOWR_ALTERA_AVALON_TIMER_CONTROL(0x3020, 0x7); // Inicia el temporizador con interrupciones habilitadas
    alt_ic_isr_register(0, 1, timer_isr, NULL, 0x0);
}

// Inicialización de las interrupciones de los botones
void init_button_interrupts() {
    // Configuración del botón de horas
    alt_ic_isr_register(PIO_KEY_3_IRQ_INTERRUPT_CONTROLLER_ID,
    					PIO_KEY_3_IRQ, PIO_KEY_3_BASE, NULL, 0x0);

    // Configuración del botón de minutos
    //alt_ic_isr_register(PIO_BUTTON_MINUTES_IRQ_INTERRUPT_CONTROLLER_ID,
    //                    PIO_BUTTON_MINUTES_IRQ, button_minutes_isr, NULL, 0x0);
}

// Actualización del tiempo actual cada segundo
void update_time(int* hours, int* minutes) {
    if (timer_flag) {
        (*minutes)++;
        if (*minutes >= 60) {
            *minutes = 0;
            (*hours)++;
            if (*hours >= 24) *hours = 0;
        }
	timer_flag = 0; // Reinicia la bandera
    }
}

// Comprobación de la alarma
void check_alarm(int current_hours, int current_minutes, int alarm_hours, int alarm_minutes) {
    if (current_hours == alarm_hours && current_minutes == alarm_minutes) {
        alt_putstr("ALARM! Time to wake up!\n");
    }
}



int main() {
    IOWR_ALTERA_AVALON_PIO_DATA(LEDS_SECONDS_LS_BASE, decoder(0));
	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_SECONDS_MS_BASE, decoder(0));
	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_MINUTES_LS_BASE, decoder(0));
	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_MINUTES_MS_BASE, decoder(0));

	init_timer(); // Inicializa el temporizador
	init_button_interrupts(); // Inicializa las interrupciones de los botones

	// Pequeña pausa para no saturar el terminal
	while(1);
    return 0;
}
