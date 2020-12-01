#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"

void startup();

void main() {
    startup();
    kprint("Bem-Vindo ao ShinOS.\n");
    kprint("digite SHUTDOWN para parar a CPU.\n> ");
}

void startup(){
    isr_install();
    irq_install();
    clear_screen();
}

void user_input(char *input) {
    if (strcmp(input, "SHUTDOWN") == 0) {
        kprint("Parando a CPU. Adeus!\n");
        asm volatile("hlt");
    }else if(strcmp(input, "CLEAR") == 0){
        clear_screen();
        kprint("Bem-Vindo ao ShinOS.\n");
        kprint("digite SHUTDOWN para parar a CPU.\n>");
    }else{
        kprint("Comando nao reconhecido!\n");
        kprint("> ");
    }
}
