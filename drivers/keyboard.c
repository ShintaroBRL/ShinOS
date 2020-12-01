#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"
#include "special_keysmap.h"

static char key_buffer[256];

#define SC_MAX 128

const char caps_sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' ','A'};

static void keyboard_callback(registers_t regs) {
    /* The PIC leaves us the scancode in port 0x60 */
    u8 scancode = port_byte_in(0x60);
    
    if (scancode > SC_MAX) return;

    if (scancode == BACKSPACE) {
        backspace(key_buffer);
        kprint_backspace();
    } else if (scancode == ENTER) {
        kprint("\n");
        user_input(key_buffer); /* kernel-controlled function */
        key_buffer[0] = '\0';
    } else if (scancode == ARROW_LEFT) {
        kprint("left arrow");
    } else if (scancode == F1) {
        kprint("F1");
    } else if (scancode == F2) {
        kprint("F2");
    } else if (scancode == F3) {
        kprint("F3");
    } else if (scancode == F4) {
        kprint("F4");
    } else if (scancode == F5) {
        kprint("F5");
    } else if (scancode == F6) {
        kprint("F6");
    } else if (scancode == F7) {
        kprint("F7");
    } else if (scancode == F8) {
        kprint("F8");
    } else if (scancode == F9) {
        kprint("F9");
    } else if (scancode == F10) {
        kprint("F10");
    } else if (scancode == F11) {
        kprint("F11");
    } else if (scancode == F12) {
        kprint("F12");
    } else if (scancode == CAPSLOCK) {
        kprint("Caps_Lock");
    } else {
        char letter = caps_sc_ascii[(int)scancode];
        /* Remember that kprint only accepts char[] */
        char str[2] = {letter, '\0'};
        append(key_buffer, letter);
        kprint(str);
    }
    
    UNUSED(regs);
}

void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback); 
}
