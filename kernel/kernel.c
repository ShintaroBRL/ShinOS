#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include <stdint.h>
#include "../libc/paging.h"

void kernel_main() {
    isr_install();
    irq_install();

    asm("int $2");
    asm("int $3");

    clear_screen();
    initialise_paging();

    u32int *ptr = (u32int*)0xA0000000;
    u32int do_page_fault = *ptr;

    kprint("Bem-Vindo ao ShinOS!\n"
        "END -> shutdown  PAGE -> request a kmalloc()\n\nShinOS>: ");
}

void user_input(char *input) {
    if (strcmp(input, "END") == 0) {
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } else if (strcmp(input, "PAGE") == 0) {
        kprint("malock in dev.");
    } else if (strcmp(input, "CLEAR") == 0) {
        clear_screen();
    }else{
        kprint("Command not found");
    }
    kprint("\nShinOS>: ");
}
