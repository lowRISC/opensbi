/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2019 Western Digital Corporation or its affiliates.
 *
 * Authors:
 *   Anup Patel <anup.patel@wdc.com>
 */

#include <sbi/sbi_ecall_interface.h>
/* for checksum */
#include "hash-md5.c"

#define wfi()						\
do {							\
	__asm__ __volatile__ ("wfi" ::: "memory");	\
} while (0)

#define putc sbi_ecall_console_putc
#define puts sbi_ecall_console_puts

void putn(unsigned long n)
{
  if (n > 9) putn(n / 10);
  putc(n % 10 + '0');
}

void putx(unsigned long n)
{
  if (n > 15) putx(n >> 4);
  putc("0123456789ABCDEF"[n&15]);
}

void putadr(void *adr)
{
  putx((unsigned long)adr);
}

void puthash(char *ptr, size_t siz)
{
  uint32_t *_ptr = (uint32_t *)ptr;
  uint8_t *hash_value = hash_buf(ptr, siz);
  puts("md5(");
  putadr(_ptr);
  putc(',');
  putn(siz);
  puts(") = ");
  puts((char *)hash_value);
  putc('\n');
#ifdef VERBOSE
  for (int i = 0; i < 10; i++)
    {
    putadr(_ptr+i);
    puts(": ");
    putx(_ptr[i]);
    putc('\n');
    }
#endif  
}

void test_main(unsigned long a0, unsigned long a1)
{
  extern char _dtb[];
  extern char _edtb[];
  extern char _kernel[];
  extern char _ekernel[];
  int dsiz = _edtb - _dtb;
  int ksiz = _ekernel - _kernel;
  char *kbase = (char *)0x80200000;
  int slack = _dtb-kbase-ksiz;
  void (*kptr)(uint64_t, void *) = (void*)kbase;
  
  puts("\nTest payload running\n");
  putadr(test_main);
  putc('\n');
  putadr(_dtb);
  putc('\n');
  putadr(_kernel);
  putc('\n');
  memcpy(kbase, _kernel, ksiz); // copy to the correct address
  memset(kbase+ksiz, 0, slack);
  puthash(_dtb, dsiz);
  puthash(kbase, ksiz);
  asm volatile ("fence.i");
  puts("Calling kernel now\n");
  
  kptr(0, _dtb);
  while (1)
    wfi();
}
