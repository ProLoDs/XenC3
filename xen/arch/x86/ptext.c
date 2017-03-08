/*
 * ptext.c
 *
 *  Created on: 03.02.2017
 *      Author: bej
 */

#include <xen/ptext.h>
#include <xen/sched.h>
#include <xen/domain.h>



/*
long retreive_mods(){

}

long get_new_accessed(struct vcpu *virtcpu){
	unsigned long cr3 = virtcpu->arch.cr3;
	return 0;
}

void walk_guest_pages(unsigned long cr3)
{
    unsigned long pfn, mfn = read_cr3() >> PAGE_SHIFT;
    l4_pgentry_t l4e, *l4t;
    l3_pgentry_t l3e, *l3t;
    l2_pgentry_t l2e, *l2t;
    l1_pgentry_t l1e, *l1t;

    printk("Pagetable walk from %016lx:\n", addr);
    if ( !is_canonical_address(addr) )
        return;

    l4t = map_domain_page(mfn);
    l4e = l4t[l4_table_offset(addr)];
    unmap_domain_page(l4t);
    mfn = l4e_get_pfn(l4e);
    pfn = mfn_valid(mfn) && machine_to_phys_mapping_valid ?
          get_gpfn_from_mfn(mfn) : INVALID_M2P_ENTRY;
    printk(" L4[0x%03lx] = %"PRIpte" %016lx\n",
           l4_table_offset(addr), l4e_get_intpte(l4e), pfn);
    if ( !(l4e_get_flags(l4e) & _PAGE_PRESENT) ||
         !mfn_valid(mfn) )
        return;

    l3t = map_domain_page(mfn);
    l3e = l3t[l3_table_offset(addr)];
    unmap_domain_page(l3t);
    mfn = l3e_get_pfn(l3e);
    pfn = mfn_valid(mfn) && machine_to_phys_mapping_valid ?
          get_gpfn_from_mfn(mfn) : INVALID_M2P_ENTRY;
    printk(" L3[0x%03lx] = %"PRIpte" %016lx%s\n",
           l3_table_offset(addr), l3e_get_intpte(l3e), pfn,
           (l3e_get_flags(l3e) & _PAGE_PSE) ? " (PSE)" : "");
    if ( !(l3e_get_flags(l3e) & _PAGE_PRESENT) ||
         (l3e_get_flags(l3e) & _PAGE_PSE) ||
         !mfn_valid(mfn) )
        return;

    l2t = map_domain_page(mfn);
    l2e = l2t[l2_table_offset(addr)];
    unmap_domain_page(l2t);
    mfn = l2e_get_pfn(l2e);
    pfn = mfn_valid(mfn) && machine_to_phys_mapping_valid ?
          get_gpfn_from_mfn(mfn) : INVALID_M2P_ENTRY;
    printk(" L2[0x%03lx] = %"PRIpte" %016lx %s\n",
           l2_table_offset(addr), l2e_get_intpte(l2e), pfn,
           (l2e_get_flags(l2e) & _PAGE_PSE) ? "(PSE)" : "");
    if ( !(l2e_get_flags(l2e) & _PAGE_PRESENT) ||
         (l2e_get_flags(l2e) & _PAGE_PSE) ||
         !mfn_valid(mfn) )
        return;

    l1t = map_domain_page(mfn);
    l1e = l1t[l1_table_offset(addr)];
    unmap_domain_page(l1t);
    mfn = l1e_get_pfn(l1e);
    pfn = mfn_valid(mfn) && machine_to_phys_mapping_valid ?
          get_gpfn_from_mfn(mfn) : INVALID_M2P_ENTRY;
    printk(" L1[0x%03lx] = %"PRIpte" %016lx\n",
           l1_table_offset(addr), l1e_get_intpte(l1e), pfn);
}*/




