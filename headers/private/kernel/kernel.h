/*
 Telif hakki Fegeya 2019
 */
#ifndef _KERNEL_KERNEL_H
#define _KERNEL_KERNEL_H


#include <config/types.h>

#include <arch_kernel.h>
#include <arch_config.h>


#ifndef KERNEL_LOAD_BASE
#	define KERNEL_LOAD_BASE		KERNEL_BASE
#endif

// macro to check whether an address is in the kernel address space (avoid
// always-true checks)
#if KERNEL_BASE == 0
#	define IS_KERNEL_ADDRESS(x)		((addr_t)(x) <= KERNEL_TOP)
#elif KERNEL_TOP == __HAIKU_ADDR_MAX
#	define IS_KERNEL_ADDRESS(x)		((addr_t)(x) >= KERNEL_BASE)
#else
#	define IS_KERNEL_ADDRESS(x) \
		((addr_t)(x) >= KERNEL_BASE && (addr_t)(x) <= KERNEL_TOP)
#endif

// Buffers passed in from user-space shouldn't point into the kernel.
#if USER_BASE == 0
#	define IS_USER_ADDRESS(x)		((addr_t)(x) <= USER_TOP)
#elif USER_TOP == __HAIKU_ADDR_MAX
#	define IS_USER_ADDRESS(x)		((addr_t)(x) >= USER_BASE)
#else
#	define IS_USER_ADDRESS(x) \
		((addr_t)(x) >= USER_BASE && (addr_t)(x) <= USER_TOP)
#endif

#define DEBUG_KERNEL_STACKS
	// Note, debugging kernel stacks doesn't really work yet. Since the
	// interrupt will also try to use the stack on a page fault, all
	// you get is a double fault.
	// At least, you then know that the stack overflows in this case :)

/** Size of the kernel stack */
#ifdef B_HAIKU_64_BIT
	#define KERNEL_STACK_SIZE		(B_PAGE_SIZE * 4)	// 16 kB
#else
	#define KERNEL_STACK_SIZE		(B_PAGE_SIZE * 3)	// 12 kB
#endif

#ifdef DEBUG_KERNEL_STACKS
#	define KERNEL_STACK_GUARD_PAGES	1
#else
#	define KERNEL_STACK_GUARD_PAGES	0
#endif

/** Size of the environmental variables space for a process */
#define ENV_SIZE	(B_PAGE_SIZE * 8)


#define ROUNDDOWN(a, b)	(((a) / (b)) * (b))
#define ROUNDUP(a, b)	ROUNDDOWN((a) + (b) - 1, b)


#define CHECK_BIT(a, b) ((a) & (1 << (b)))
#define SET_BIT(a, b) ((a) | (1 << (b)))
#define CLEAR_BIT(a, b) ((a) & (~(1 << (b))))
#define GET_BIT(a, b) ((a & b) != 0)
#define TOGGLE_BIT(a, b) (a ^= b)


/* during kernel startup, interrupts are disabled (among other things) */
extern bool gKernelStartup;
extern bool gKernelShutdown;

#ifdef __cplusplus
extern "C" {
#endif

status_t system_shutdown(bool reboot);
status_t _user_shutdown(bool reboot);
status_t _user_recovery_mode(bool reboot);
#ifdef __cplusplus
}
#endif

#endif	/* _KERNEL_KERNEL_H */
