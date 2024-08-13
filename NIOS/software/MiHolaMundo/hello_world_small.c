#include "system.h"
#include "altera_avalon_timer_regs.h"
#include "alt_types.h"
#include "sys/alt_irq.h"
#include  <stdio.h>
#include <stdlib.h>

// Variables globales
volatile int timer_flag = 0; // Indicador de interrupción del temporizador
int current_hours = 0, current_minutes = 0;
int alarm_hours = 6, alarm_minutes = 30; // Hora de alarma predeterminada
volatile unsigned short *led1 = (short *) 0x3000;

// ISR del temporizador
void timer_isr(void* context, alt_u32 id) {
    IOWR_ALTERA_AVALON_TIMER_STATUS(0x3020, 0); // Limpia la interrupción
    timer_flag = 1; // Marca que ha pasado un segundo




    // Aquí podrías llamar a set_time() según la entrada del usuario para cambiar la hora
    update_time(&current_hours, &current_minutes); // Actualiza la hora actual




}


// Inicialización del temporizador
void init_timer() {
    // Configuración del temporizabdor para generar una interrupción cada segundo
    IOWR_ALTERA_AVALON_TIMER_PERIODL(0x3020, 33333333*2 & 0xFFFF); // Valor bajo del periodo para 1 segundo
    IOWR_ALTERA_AVALON_TIMER_PERIODH(0x3020, (33333333*2>>16)&0xFFFF); // Valor alto del periodo
    IOWR_ALTERA_AVALON_TIMER_CONTROL(0x3020, 0x7); // Inicia el temporizador con interrupciones habilitadas
    alt_ic_isr_register(0, 1, timer_isr, NULL, 0x0);
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
        // Para pruebas, imprime la hora actual
    char str[12];
    alt_printf("Hour and minutes");
    itoa(current_hours, str, 10);
	alt_printf(str);
    itoa(current_minutes, str, 10);
    alt_printf(str);
	alt_printf("\n");
        timer_flag = 0; // Reinicia la bandera
    }
}



int main() {
    init_timer(); // Inicializa el temporizador





        // Simulación de trabajo en el bucle principal
        // Aquí podrías añadir más lógica o funcionalidades adicionales

        // Pequeña pausa para no saturar el terminal
        while(1);
    return 0;
}
