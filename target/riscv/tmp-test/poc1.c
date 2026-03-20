/*
 * PoC to test the new CSRs and instructions
 *
 * clang --target=riscv64-linux-gnu -fuse-ld=lld -nostdlib -march=rv64gc -static -O0 poc1.c -o poc1
 */

#include <stdint.h>
#include "../cpu_bits.h"

#define _STR(x) #x
#define STR(x) _STR(x)

#define write_msignkey(val) asm volatile ("csrw " STR(CSR_MSIGN_KEY) ", %0" :: "r"(val))
#define write_msigncfg(val) asm volatile ("csrw " STR(CSR_MSIGN_CFG) ", %0" :: "r"(val))

#define msign(rd, rs1, imm) \
    asm volatile (".insn i 0x0B, 0, %0, %1, %2" : "=r"(rd) : "r"(rs1), "i"(imm))

#define mverify(rd, rs1, imm) \
    asm volatile (".insn i 0x0B, 1, %0, %1, %2" : "=r"(rd) : "r"(rs1), "i"(imm))

void print_msg(const char *msg)
{
    int len = 0;
    while (*msg++) len++;

    register long a0 asm("a0") = 1;      // STDOUT
    register long a1 asm("a1") = (long)msg;
    register long a2 asm("a2") = len;
    register long a7 asm("a7") = 64;     // sys_write
    asm volatile ("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7) : "memory");
}

void sys_exit(int code)
{
    register long a0 asm("a0") = code;
    register long a7 asm("a7") = 93;     // sys_exit
    asm volatile ("ecall" : "+r"(a0) : "r"(a7) : "memory");
}

void _start()
{
    char buf[0x20];
    for (int i = 0; i < sizeof(buf); i++)
        buf[i] = 0xba;

    uint64_t key = 0xDEADBEEFCAFEBABE;
    write_msignkey(key);

    uint64_t cfg = 0x1;
    write_msigncfg(cfg);

    uint64_t sign_val = 0;
    msign(sign_val, &buf, sizeof(buf));

    uint64_t tmp = sign_val;
    mverify(tmp, &buf, sizeof(buf));

    if (tmp)
        print_msg("Verified\n");
    else
        print_msg("Failed\n");

    sys_exit(0);
}
