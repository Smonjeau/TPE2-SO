#include <time.h>
#include <stdint.h>
#include <keyboardDriver.h>
#include <ConsoleManager.h>

static void int_20();
static void int_21();

void irqDispatcher(uint64_t irq,void * rsp) {

	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21(rsp);
	}
	return;
}

void int_20() {
	timer_handler();
}

void int_21(void*  rsp ) {
	//keyboard handler
	keyboard_handler(rsp);
}
