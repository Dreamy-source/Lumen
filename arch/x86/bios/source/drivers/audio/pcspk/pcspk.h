#ifndef PC_SPEAKER_H
#define PC_SPEAKER_H

#include "common/baselib.h"
#include "interrupts/legacy/handlers/IRQ0/irq0-sleep.h"

static inline void play_sound(uint32_t freq) {
	uint32_t div;
	uint8_t tmp;
 
	div = 1193180 / freq;
	outb(0x43, 0xb6);
	outb(0x42, (uint8_t) (div) );
	outb(0x42, (uint8_t) (div >> 8));

	tmp = inb(0x61);
 	if (tmp != (tmp | 3)) {
		outb(0x61, tmp | 3);
	}
}

static inline void stop_sound()
{
    uint8_t tmp = inb(0x61) & 0xFC;
	outb(0x61, tmp);
}

static inline void play_note(uint32_t freq, uint32_t duration_ms) {
    play_sound(freq);
    sleep(duration_ms);
    stop_sound();
    sleep(20);
}

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

static inline void play_happy_birthday(void) {
    play_note(NOTE_C4, 200);
    play_note(NOTE_C4, 100);
    play_note(NOTE_D4, 400);
    play_note(NOTE_C4, 400);
    play_note(NOTE_F4, 400);
    play_note(NOTE_E4, 800);
    
    play_note(NOTE_C4, 200);
    play_note(NOTE_C4, 100);
    play_note(NOTE_D4, 400);
    play_note(NOTE_C4, 400);
    play_note(NOTE_G4, 400);
    play_note(NOTE_F4, 800);
    
    play_note(NOTE_C4, 200);
    play_note(NOTE_C4, 100);
    play_note(NOTE_C5, 400);
    play_note(NOTE_A4, 400);
    play_note(NOTE_F4, 400);
    play_note(NOTE_E4, 400);
    play_note(NOTE_D4, 800);
    
    play_note(NOTE_B4, 200);
    play_note(NOTE_B4, 100);
    play_note(NOTE_A4, 400);
    play_note(NOTE_F4, 400);
    play_note(NOTE_G4, 400);
    play_note(NOTE_F4, 800);
}

#endif