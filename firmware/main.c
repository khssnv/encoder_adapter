/* 
 * Author : Alisher A. Khassanov
 * Email  : a.khssnv@gmail.com
 * 
 * Max input freq: F_CPU / 2.5
 * Input pin     : T1 / PD5
 * Serial config : 115200, 8n1
 * Output period : ~6,4 ms
 * Output freq   : ~155,25 Hz
 * Output format : `#<uint8 seq>:<uint16 pulses>:<crc8 of all the previous>\r\n` (ASCII base16)
 * Output example: `#d1:0006:b9\r\n` means sequence 29, 6 pulses registered, crc8 is 185
 */

#define F_CPU           16000000L
#define UART_BAUDRATE   115200
#define MSG_LENGTH      10

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/crc16.h>
#include <stdio.h>

// https://github.com/jnk0le/AVR-UART-lib/commit/5705dfd338cc5eeba6804a2760b1dd23e22acec5
#include "usart/usart.c"

volatile uint8_t seq = 0;
volatile uint8_t seqs_new = 0;
volatile uint16_t counter = 0;
char msg_buff[MSG_LENGTH] = {0};

ISR(TIMER0_COMPA_vect)
{
    seq++;
    seqs_new++;
    counter = TCNT1; TCNT1 = 0; // keep it atomic
}

void
setup()
{
    // ---
    // Timer/Counter1 is encoder input
    DDRD &= ~(1<<DDD5); // set pin T1 (PD5) input mode
    PORTD |= (1<<PORTD5); // enable pull-up
    TCCR1B |= (1<<CS12) | (1<<CS11) | (1<<CS10); // external clock source
    TCNT1 = 0; // reset counter

    // ---
    // UART for output
    uart_set_FrameFormat(USART_8BIT_DATA | USART_1STOP_BIT | USART_NO_PARITY | USART_ASYNC_MODE); // default settings
    uart_init(BAUD_CALC(UART_BAUDRATE));
    stdout = &uart0_out; // attach UART stream to stdout
    
    // ---
    // Timer/Counter0 interrupts to capture counter value and send it to UART
    TCCR0A |= (1<<WGM01); // CTC mode
    TCCR0B |= (1<<CS02) | (1<<CS00); // clk_io / 1024 from prescaler
    OCR0A = 100; // an interrupt each 6,400 ms => 156,25 Hz
    TIMSK0 |= (1<<OCIE0A); // Timer/Counter0 compare interrupt
    TCNT0 = 0; // reset timer
}

int
main()
{
    setup();
    sei();

    while(1)
    {
        if(seqs_new) // seqs_new > 1 means you lost one or more seqs
        {
            seqs_new = 0;
            sprintf(msg_buff, "#%02x:%04x:", seq, counter);
            uint8_t crc8 = 0, i;
            for(i = 0; i < MSG_LENGTH; i++)
                crc8 = _crc8_ccitt_update(crc8, msg_buff[i]);
            printf("%s%02x\n", msg_buff, crc8);
        }
    }
}
