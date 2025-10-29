/*
 * ===================================================================
 * PROYECTO:   Lector, Ordenador y Visualizador de 5 N�meros
 * PIC:        PIC16F887
 * COMPILADOR: XC8
 * OSCILADOR:  Interno a 8 MHz
 * ===================================================================
 */

// --- CONFIGURACI�N 1 ---
#pragma config FOSC = INTRC_NOCLKOUT // Oscilador Interno (INTOSC), sin salida de reloj
#pragma config WDTE = OFF       // Watchdog Timer deshabilitado
#pragma config PWRTE = OFF      // Power-up Timer deshabilitado
#pragma config MCLRE = ON       // Pin MCLR habilitado como Reset
#pragma config CP = OFF         // Protecci�n de c�digo deshabilitada
#pragma config CPD = OFF        // Protecci�n de datos deshabilitada
#pragma config BOREN = ON       // Brown-out Reset habilitado
#pragma config IESO = ON        // Internal/External Switchover habilitado
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor habilitado
#pragma config LVP = OFF        // Low-Voltage Programming deshabilitado

// --- CONFIGURACI�N 2 ---
#pragma config BOR4V = BOR40V   // Brown-out Reset a 4.0V
#pragma config WRT = OFF        // Write protection deshabilitada

#include <xc.h>

#define _XTAL_FREQ 8000000    // Frecuencia para __delay_ms()

/**
 * @brief Ordena un arreglo de unsigned char usando el m�todo de burbuja (ascendente).
 */
void bubble_sort(unsigned char arr[], int n) {
    int i, j;
    unsigned char temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void main(void) {
    
    // --- Variables ---
    unsigned char lista_numeros[10];
    unsigned char contador = 0;
    unsigned char i;

    // --- INICIALIZACI�N DE HARDWARE ---
    
    OSCCON = 0b01110000;  // 1. Oscilador interno a 8 MHz
    
    // 2. Configurar Pines como Digitales (�CR�TICO!)
    ANSEL = 0x00;         
    ANSELH = 0x00;        // <-- Pone RB, RC, RD como digitales.

    // 3. Configurar Direcci�n de Puertos (TRIS)
    TRISD = 0xFF;         // PORTD (RD0-RD7) como ENTRADAS
    TRISB = 0x00;         // PORTB (RB0-RB7) como SALIDAS
    TRISC = 0x01;         // RC0 como ENTRADA, RC1/RC2 como SALIDAS

    // 4. Limpiar Puertos
    PORTD = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;

    // ===============================================
    //           FASE 1: INGRESO DE N�MEROS
    // ===============================================
    
    while (contador < 10) {
        
        while(PORTCbits.RC0 == 0); // 1. Esperar presi�n de bot�n
        
        lista_numeros[contador] = ~PORTD; // 2. Leer DIP Switch
        
        contador++; // 3. Incrementar contador
        
        PORTCbits.RC1 = 1; // 4. LED de confirmaci�n ON
        __delay_ms(1000);  // 5. Esperar 1s
        PORTCbits.RC1 = 0; // 6. LED de confirmaci�n OFF
        
        while(PORTCbits.RC0 == 1); // 7. Esperar a soltar el bot�n
    }
    
    // ===============================================
    //           FASE 2: ORDENAMIENTO
    // ===============================================
    
    PORTCbits.RC2 = 1; // 1. Encender LED de "terminado"
    
    bubble_sort(lista_numeros, 10); // 2. Ordenar el arreglo
    
    // ===============================================
    //        FASE 3: VISUALIZACI�N DE DATOS
    // ===============================================
    
    while(1) { // Bucle infinito
        
        for (i = 0; i < 10; i++) {
            
            // 1. Mostrar el n�mero ordenado en los LEDs
            PORTB = lista_numeros[i];
            
            // 2. Esperar 2 segundos
            __delay_ms(2000);
        }
    }
    
    return;
}