#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"
#include <stdint.h>
#include "keymap.h"

static char key_buffer[256];
static int CapsLockStatus = 0;

static int backspace_enabled_times = 0;

#define SC_MAX 58

static void keyboard_callback(registers_t *regs) {
    /* The PIC leaves us the scancode in port 0x60 */
    uint8_t scancode = port_byte_in(0x60);
    
    if (scancode > SC_MAX) return;
    if (scancode == BACKSPACE) {
        if(backspace_enabled_times > 0){
            backspace(key_buffer);
            kprint_backspace();
            backspace_enabled_times--;
        }
    } else if (scancode == ENTER) {
        kprint("\n");
        user_input(key_buffer); /* kernel-controlled function */
        key_buffer[0] = '\0';
        backspace_enabled_times = 0;
    } else if (scancode == CAPSLUCK) {
        /* Toggle caps lock */
        if (CapsLockStatus)
            CapsLockStatus = 0;
        else
            CapsLockStatus = 1;
    } else {
        backspace_enabled_times++;
        char letter = sc_ascii[(int)scancode];
        if (CapsLockStatus == 0)
            letter = Lsc_ascii[(int)scancode];
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