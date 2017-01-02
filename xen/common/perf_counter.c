/*
 * perf_counter.c
 *
 *  Created on: 23.12.2016
 *      Author: Maximilian Zeiser
 */

//#include <linux/module.h>   /* Needed by all modules */
//#include <linux/kernel.h>   /* Needed for KERN_INFO */
#include <xen/perf_counter.h>
#include <xen/types.h>

static inline void rtxen_clear_msr( uint32_t ecx)
{
    /*clear counter first*/
   __asm__ __volatile__ ("movl %0, %%ecx\n\t"
        "xorl %%edx, %%edx\n\t"
        "xorl %%eax, %%eax\n\t"
        "wrmsr\n\t"
        : /* no outputs */
        : "m" (ecx)
        : "eax", "ecx", "edx" /* all clobbered */);
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

    uint32_t ecx;
    uint64_t event = 0;
    SET_MSR_USR_BIT(event);
    SET_MSR_OS_BIT(event);
    event |= MSR_ENFLAG;
    event |= (1<<20); //INT bit: counter overflow
    switch(l)
    {
    case(L1_I):

            SET_EVENT_MASK(event, L1I_ALLMISS_EVENT, L1I_ALLMISS_MASK);
            ecx = PERFEVTSEL0;
    		break;
    case(L1_D):
            SET_EVENT_MASK(event, L1D_LDMISS_EVENT, L1D_LDMISS_MASK);
            ecx = PERFEVTSEL1;
       		break;
    case(L2):
            SET_EVENT_MASK(event, L2_ALLMISS_EVENT, L2_ALLMISS_MASK);
            ecx = PERFEVTSEL0;
    		break;
    case(L3):
	        SET_EVENT_MASK(event, L3_ALLMISS_EVENT, L3_ALLMISS_MASK);
	        ecx = PERFEVTSEL3;
    		break;
    default:
    	    return;

    }
    rtxen_clear_msr(ecx);
    wrmsr(ecx,event);
}
uint64_t stop_counter(enum cache_level l)
{

    uint32_t ecx;
    uint64_t event = 0;
    uint64_t ret = 0;

	SET_MSR_USR_BIT(event);
	SET_MSR_OS_BIT(event);

	event |= (1<<20); //INT bit: counter overflow
	event &= (~MSR_ENFLAG);
    switch(l)
    {
    case(L1_I):
            SET_EVENT_MASK(event, L1I_ALLMISS_EVENT, L1I_ALLMISS_MASK);
			ecx = PERFEVTSEL0;
			wrmsr(ecx,event);
			ecx = PMC0;
			ret = rdmsr(ecx);
    		break;
    case(L1_D):
            SET_EVENT_MASK(event, L1D_LDMISS_EVENT, L1D_LDMISS_MASK);
   			ecx = PERFEVTSEL1;
   			wrmsr(ecx,event);
   			ecx = PMC1;
   			ret = rdmsr(ecx);
    		break;
    case(L2):

			SET_EVENT_MASK(event, L2_ALLMISS_EVENT, L2_ALLMISS_MASK);
			ecx = PERFEVTSEL0;
			wrmsr(ecx,event);
			ecx = PMC0;
			ret = rdmsr(ecx);
    		break;
    case(L3):
			SET_EVENT_MASK(event, L3_ALLMISS_EVENT, L3_ALLMISS_MASK);
			ecx = PERFEVTSEL3;
			wrmsr(ecx,event);
			ecx = PMC3;
			ret = rdmsr(ecx);
    		break;

    }
    return ret;
}
static inline void delay(void )
{
    char tmp[1000];
    int i;
    for( i = 0; i < 1000; i++ )
    {
        tmp[i] = i * 2;
    }
}
