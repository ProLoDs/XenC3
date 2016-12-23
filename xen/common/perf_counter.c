/*
 * perf_counter.c
 *
 *  Created on: 23.12.2016
 *      Author: Maximilian Zeiser
 */

//#include <linux/module.h>   /* Needed by all modules */
#include <linux/kernel.h>   /* Needed for KERN_INFO */
#include "perf_counter.h"


static inline void rtxen_write_msr(uint32_t eax, uint32_t ecx)
{
    /*clear counter first*/
   __asm__ __volatile__ ("movl %0, %%ecx\n\t"
        "xorl %%edx, %%edx\n\t"
        "xorl %%eax, %%eax\n\t"
        "wrmsr\n\t"
        : /* no outputs */
        : "m" (ecx)
        : "eax", "ecx", "edx" /* all clobbered */);

   eax |= MSR_ENFLAG;

   __asm__("movl %0, %%ecx\n\t" /* ecx contains the number of the MSR to set */
        "xorl %%edx, %%edx\n\t"/* edx contains the high bits to set the MSR to */
        "movl %1, %%eax\n\t" /* eax contains the log bits to set the MSR to */
        "wrmsr\n\t"
        : /* no outputs */
        : "m" (ecx), "m" (eax)
        : "eax", "ecx", "edx" /* clobbered */);
}
void wrmsr(uint32_t idx, uint64_t v)
{
    asm volatile (
        "wrmsr"
        : : "c" (idx), "a" ((uint32_t)v), "d" ((uint32_t)(v>>32)) );
}

uint64_t rdmsr(uint32_t idx)
{
    uint32_t lo, hi;

    asm volatile (
        "rdmsr"
        : "=a" (lo), "=d" (hi) : "c" (idx) );

    return (lo | ((uint64_t)hi << 32));
}

void start_counter(enum cache_level l)
{

    uint32_t eax, edx, ecx;
    switch(l)
    {
    case(L1):

    		break;
    case(L2):
    		break;
    case(L3):
				eax = 0;
		        SET_MSR_USR_BIT(eax);
		        SET_MSR_OS_BIT(eax);
		        SET_EVENT_MASK(eax, L3_ALLMISS_EVENT, L3_ALLMISS_MASK);
		        eax |= MSR_ENFLAG;
		        eax |= (1<<20); //INT bit: counter overflow
		        ecx = PERFEVTSEL2;
		        printk(KERN_INFO "before wrmsr: eax=%#010x, ecx=%#010x\n", eax, ecx);
		        rtxen_write_msr(eax, ecx);
		        printk(KERN_INFO "after wrmsr: eax=%#010x, ecx=%#010x\n", eax, ecx);
		        printk(KERN_INFO "L3 all request set MSR PMC2\n");
    		break;

    }
}
uint64_t stop_counter(enum cache_level l)
{

    uint32_t eax, edx, ecx;
    uint64_t ret = 0;
    switch(l)
    {
    case(L1):

    		break;
    case(L2):
    		break;
    case(L3):
			eax &= (~MSR_ENFLAG);
    		rtxen_write_msr(eax, ecx);
		    printk(KERN_INFO "stop the counter, eax=%#010x\n", eax);
		    ecx = PMC2;
		    eax = 1;
		    edx = 2;
		    printk(KERN_INFO "rdmsr: ecx=%#010x\n", ecx);
		    rdmsr(&ecx, &eax, &edx); /*need to pass into address!*/
		    ret = ( ((uint64_t) edx << 32) | eax );
    		break;

    }
    return ret;
}
