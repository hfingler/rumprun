/*-
 * Copyright (c) 2014 Antti Kantee.  All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <hw/types.h>
#include <hw/kernel.h>
#include <hw/multiboot.h>

#include <bmk-core/core.h>
#include <bmk-core/mainthread.h>
#include <bmk-core/sched.h>
#include <bmk-core/printf.h>

//for time
#include <bmk-core/platform.h>
#include <arch/x86/cons.h>

void
x86_boot(struct multiboot_info *mbi)
{
	cons_init();

	bmk_printf("rump kernel bare metal bootstrap\n\n");

	cpu_init();

	uint32_t x[100];
	//time before sched
	for (int i = 0 ; i < 100 ; i++) {
		bmk_time_t st = bmk_platform_cpu_clock_monotonic();
		serialcons_putc('a')
		bmk_time_t end = bmk_platform_cpu_clock_monotonic();
		bmk_time_t dif = end-st;

		x[i] = dif;
	}
	
	bmk_printf("\n\n\n\n\n");
	for (int i = 0 ; i < 100 ; i++)
		bmk_printf("time: %u\n\n", x[i]);
	//end of timing

	bmk_sched_init();
	multiboot(mbi);

	spl0();

	bmk_sched_startmain(bmk_mainthread, multiboot_cmdline);
}
