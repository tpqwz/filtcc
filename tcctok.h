/*********************************************************************/
/* keywords */
     DEF(TOK_IF, "if")
     DEF(TOK_ELSE, "else")
     DEF(TOK_WHILE, "while")
     DEF(TOK_FOR, "for")
     DEF(TOK_DO, "do")
     DEF(TOK_CONTINUE, "continue")
     DEF(TOK_BREAK, "break")
     DEF(TOK_RETURN, "return")
     DEF(TOK_GOTO, "goto")
     DEF(TOK_SWITCH, "switch")
     DEF(TOK_CASE, "case")
     DEF(TOK_DEFAULT, "default")
     DEF(TOK_ASM1, "asm")
     DEF(TOK_ASM2, "__asm")
     DEF(TOK_ASM3, "__asm__")

     DEF(TOK_EXTERN, "extern")
     DEF(TOK_STATIC, "static")
     DEF(TOK_UNSIGNED, "unsigned")
     DEF(TOK__Atomic, "_Atomic")
     DEF(TOK_CONST1, "const")
     DEF(TOK_CONST2, "__const") /* gcc keyword */
     DEF(TOK_CONST3, "__const__") /* gcc keyword */
     DEF(TOK_VOLATILE1, "volatile")
     DEF(TOK_VOLATILE2, "__volatile") /* gcc keyword */
     DEF(TOK_VOLATILE3, "__volatile__") /* gcc keyword */
     DEF(TOK_REGISTER, "register")
     DEF(TOK_SIGNED1, "signed")
     DEF(TOK_SIGNED2, "__signed") /* gcc keyword */
     DEF(TOK_SIGNED3, "__signed__") /* gcc keyword */
     DEF(TOK_AUTO, "auto")
     DEF(TOK_INLINE1, "inline")
     DEF(TOK_INLINE2, "__inline") /* gcc keyword */
     DEF(TOK_INLINE3, "__inline__") /* gcc keyword */
     DEF(TOK_RESTRICT1, "restrict")
     DEF(TOK_RESTRICT2, "__restrict")
     DEF(TOK_RESTRICT3, "__restrict__")
     DEF(TOK_EXTENSION, "__extension__") /* gcc keyword */
     DEF(TOK_THREAD_LOCAL, "_Thread_local") /* C11 thread-local storage */
     DEF(TOK___thread, "__thread") /* GCC thread-local storage extension */

     DEF(TOK_GENERIC, "_Generic")
     DEF(TOK_STATIC_ASSERT, "_Static_assert")

     DEF(TOK_VOID, "void")
     DEF(TOK_CHAR, "char")
     DEF(TOK_INT, "int")
     DEF(TOK_FLOAT, "float")
     DEF(TOK_DOUBLE, "double")
     DEF(TOK_BOOL, "_Bool")
     DEF(TOK_COMPLEX, "_Complex")
     DEF(TOK_SHORT, "short")
     DEF(TOK_LONG, "long")
     DEF(TOK_STRUCT, "struct")
     DEF(TOK_UNION, "union")
     DEF(TOK_TYPEDEF, "typedef")
     DEF(TOK_ENUM, "enum")
     DEF(TOK_SIZEOF, "sizeof")
     DEF(TOK_ATTRIBUTE1, "__attribute")
     DEF(TOK_ATTRIBUTE2, "__attribute__")
     DEF(TOK_ALIGNOF1, "__alignof")
     DEF(TOK_ALIGNOF2, "__alignof__")
     DEF(TOK_ALIGNOF3, "_Alignof")
     DEF(TOK_ALIGNAS, "_Alignas")
     DEF(TOK_TYPEOF1, "typeof")
     DEF(TOK_TYPEOF2, "__typeof")
     DEF(TOK_TYPEOF3, "__typeof__")
     DEF(TOK_LABEL, "__label__")

/*********************************************************************/
/* the following are not keywords. They are included to ease parsing */
/* preprocessor only */
     DEF(TOK_DEFINE, "define")
     DEF(TOK_INCLUDE, "include")
     DEF(TOK_INCLUDE_NEXT, "include_next")
     DEF(TOK_IFDEF, "ifdef")
     DEF(TOK_IFNDEF, "ifndef")
     DEF(TOK_ELIF, "elif")
     DEF(TOK_ENDIF, "endif")
     DEF(TOK_DEFINED, "defined")
     DEF(TOK_UNDEF, "undef")
     DEF(TOK_ERROR, "error")
     DEF(TOK_WARNING, "warning")
     DEF(TOK_LINE, "line")
     DEF(TOK_PRAGMA, "pragma")
     DEF(TOK___LINE__, "__LINE__")
     DEF(TOK___FILE__, "__FILE__")
     DEF(TOK___DATE__, "__DATE__")
     DEF(TOK___TIME__, "__TIME__")
     DEF(TOK___FUNCTION__, "__FUNCTION__")
     DEF(TOK___VA_ARGS__, "__VA_ARGS__")
     DEF(TOK___COUNTER__, "__COUNTER__")
     DEF(TOK___HAS_INCLUDE, "__has_include")
     DEF(TOK___HAS_INCLUDE_NEXT, "__has_include_next")

/* special identifiers */
     DEF(TOK___FUNC__, "__func__")

/* special floating point values */
     DEF(TOK___NAN__, "__nan__")
     DEF(TOK___SNAN__, "__snan__")
     DEF(TOK___INF__, "__inf__")

/* attribute identifiers */
/* XXX: handle all tokens generically since speed is not critical */
     DEF(TOK_SECTION1, "section")
     DEF(TOK_SECTION2, "__section__")
     DEF(TOK_ALIGNED1, "aligned")
     DEF(TOK_ALIGNED2, "__aligned__")
     DEF(TOK_PACKED1, "packed")
     DEF(TOK_PACKED2, "__packed__")
     DEF(TOK_WEAK1, "weak")
     DEF(TOK_WEAK2, "__weak__")
     DEF(TOK_ALIAS1, "alias")
     DEF(TOK_ALIAS2, "__alias__")
     DEF(TOK_USED1, "used")
     DEF(TOK_USED2, "__used__")
     DEF(TOK_UNUSED1, "unused")
     DEF(TOK_UNUSED2, "__unused__")
     DEF(TOK_FORMAT1, "format")
     DEF(TOK_FORMAT2, "__format__")
     DEF(TOK_NODEBUG1, "nodebug")
     DEF(TOK_NODEBUG2, "__nodebug__")
     DEF(TOK_CDECL1, "cdecl")
     DEF(TOK_CDECL2, "__cdecl")
     DEF(TOK_CDECL3, "__cdecl__")
     DEF(TOK_STDCALL1, "stdcall")
     DEF(TOK_STDCALL2, "__stdcall")
     DEF(TOK_STDCALL3, "__stdcall__")
     DEF(TOK_FASTCALL1, "fastcall")
     DEF(TOK_FASTCALL2, "__fastcall")
     DEF(TOK_FASTCALL3, "__fastcall__")
     DEF(TOK_THISCALL1, "thiscall")
     DEF(TOK_THISCALL2, "__thiscall")
     DEF(TOK_THISCALL3, "__thiscall__")
     DEF(TOK_REGPARM1, "regparm")
     DEF(TOK_REGPARM2, "__regparm__")
     DEF(TOK_CLEANUP1, "cleanup")
     DEF(TOK_CLEANUP2, "__cleanup__")
     DEF(TOK_CONSTRUCTOR1, "constructor")
     DEF(TOK_CONSTRUCTOR2, "__constructor__")
     DEF(TOK_DESTRUCTOR1, "destructor")
     DEF(TOK_DESTRUCTOR2, "__destructor__")
     DEF(TOK_ALWAYS_INLINE1, "always_inline")
     DEF(TOK_ALWAYS_INLINE2, "__always_inline__")
     DEF(TOK_NOINLINE, "__noinline__")
     DEF(TOK_PURE1, "pure")
     DEF(TOK_PURE2, "__pure__")

     DEF(TOK_MODE, "__mode__")
     DEF(TOK_MODE_QI, "__QI__")
     DEF(TOK_MODE_DI, "__DI__")
     DEF(TOK_MODE_HI, "__HI__")
     DEF(TOK_MODE_SI, "__SI__")
     DEF(TOK_MODE_word, "__word__")

     DEF(TOK_DLLEXPORT, "dllexport")
     DEF(TOK_DLLIMPORT, "dllimport")
     DEF(TOK_NODECORATE, "nodecorate")
     DEF(TOK_NORETURN1, "noreturn")
     DEF(TOK_NORETURN2, "__noreturn__")
     DEF(TOK_NORETURN3, "_Noreturn")
     DEF(TOK_VISIBILITY1, "visibility")
     DEF(TOK_VISIBILITY2, "__visibility__")

     DEF(TOK_builtin_types_compatible_p, "__builtin_types_compatible_p")
     DEF(TOK_builtin_choose_expr, "__builtin_choose_expr")
     DEF(TOK_builtin_constant_p, "__builtin_constant_p")
     DEF(TOK_builtin_frame_address, "__builtin_frame_address")
     DEF(TOK_builtin_return_address, "__builtin_return_address")
     DEF(TOK_builtin_expect, "__builtin_expect")
     DEF(TOK_builtin_unreachable, "__builtin_unreachable")
     /*DEF(TOK_builtin_va_list, "__builtin_va_list")*/
#if defined TCC_TARGET_PE && defined TCC_TARGET_X86_64
     DEF(TOK_builtin_va_start, "__builtin_va_start")
#elif defined TCC_TARGET_X86_64
     DEF(TOK_builtin_va_arg_types, "__builtin_va_arg_types")
     DEF(TOK_builtin_filc_va_arg, "__builtin_filc_va_arg")
#elif defined TCC_TARGET_ARM64
     DEF(TOK_builtin_va_start, "__builtin_va_start")
     DEF(TOK_builtin_va_arg, "__builtin_va_arg")
#elif defined TCC_TARGET_RISCV64
     DEF(TOK_builtin_va_start, "__builtin_va_start")
#endif

/* atomic operations */
#define DEF_ATOMIC(ID) DEF(TOK_##__##ID, "__"#ID)
     DEF_ATOMIC(atomic_store)
     DEF_ATOMIC(atomic_load)
     DEF_ATOMIC(atomic_exchange)
     DEF_ATOMIC(atomic_compare_exchange)
     DEF_ATOMIC(atomic_fetch_add)
     DEF_ATOMIC(atomic_fetch_sub)
     DEF_ATOMIC(atomic_fetch_or)
     DEF_ATOMIC(atomic_fetch_xor)
     DEF_ATOMIC(atomic_fetch_and)
     DEF_ATOMIC(atomic_fetch_nand)
     DEF_ATOMIC(atomic_add_fetch)
     DEF_ATOMIC(atomic_sub_fetch)
     DEF_ATOMIC(atomic_or_fetch)
     DEF_ATOMIC(atomic_xor_fetch)
     DEF_ATOMIC(atomic_and_fetch)
     DEF_ATOMIC(atomic_nand_fetch)

/* pragma */
     DEF(TOK_pack, "pack")
#if !defined(TCC_TARGET_I386) && !defined(TCC_TARGET_X86_64) && \
    !defined(TCC_TARGET_ARM) && !defined(TCC_TARGET_ARM64) && \
    !defined(TCC_TARGET_RISCV64)
     /* already defined for assembler */
     DEF(TOK_ASM_push, "push")
     DEF(TOK_ASM_pop, "pop")
#endif
     DEF(TOK_comment, "comment")
     DEF(TOK_lib, "lib")
     DEF(TOK_push_macro, "push_macro")
     DEF(TOK_pop_macro, "pop_macro")
     DEF(TOK_once, "once")
     DEF(TOK_option, "option")

/* builtin functions or variables */
#ifndef TCC_ARM_EABI
     DEF(TOK_memcpy, "memcpy")
     DEF(TOK_memmove, "memmove")
     DEF(TOK_memset, "memset")
     DEF(TOK___divdi3, "__divdi3")
     DEF(TOK___moddi3, "__moddi3")
     DEF(TOK___udivdi3, "__udivdi3")
     DEF(TOK___umoddi3, "__umoddi3")
     DEF(TOK___ashrdi3, "__ashrdi3")
     DEF(TOK___lshrdi3, "__lshrdi3")
     DEF(TOK___ashldi3, "__ashldi3")
     DEF(TOK___floatundisf, "__floatundisf")
     DEF(TOK___floatundidf, "__floatundidf")
# ifndef TCC_ARM_VFP
     DEF(TOK___floatundixf, "__floatundixf")
     DEF(TOK___fixunsxfdi, "__fixunsxfdi")
# endif
     DEF(TOK___fixunssfdi, "__fixunssfdi")
     DEF(TOK___fixunsdfdi, "__fixunsdfdi")
#endif

#if defined TCC_TARGET_ARM
# ifdef TCC_ARM_EABI
     DEF(TOK_memcpy, "__aeabi_memcpy")
     DEF(TOK_memmove, "__aeabi_memmove")
     DEF(TOK_memmove4, "__aeabi_memmove4")
     DEF(TOK_memmove8, "__aeabi_memmove8")
     DEF(TOK_memset, "__aeabi_memset")
     DEF(TOK___aeabi_ldivmod, "__aeabi_ldivmod")
     DEF(TOK___aeabi_uldivmod, "__aeabi_uldivmod")
     DEF(TOK___aeabi_idivmod, "__aeabi_idivmod")
     DEF(TOK___aeabi_uidivmod, "__aeabi_uidivmod")
     DEF(TOK___divsi3, "__aeabi_idiv")
     DEF(TOK___udivsi3, "__aeabi_uidiv")
     DEF(TOK___floatdisf, "__aeabi_l2f")
     DEF(TOK___floatdidf, "__aeabi_l2d")
     DEF(TOK___fixsfdi, "__aeabi_f2lz")
     DEF(TOK___fixdfdi, "__aeabi_d2lz")
     DEF(TOK___ashrdi3, "__aeabi_lasr")
     DEF(TOK___lshrdi3, "__aeabi_llsr")
     DEF(TOK___ashldi3, "__aeabi_llsl")
     DEF(TOK___floatundisf, "__aeabi_ul2f")
     DEF(TOK___floatundidf, "__aeabi_ul2d")
     DEF(TOK___fixunssfdi, "__aeabi_f2ulz")
     DEF(TOK___fixunsdfdi, "__aeabi_d2ulz")
# else
     DEF(TOK___modsi3, "__modsi3")
     DEF(TOK___umodsi3, "__umodsi3")
     DEF(TOK___divsi3, "__divsi3")
     DEF(TOK___udivsi3, "__udivsi3")
     DEF(TOK___floatdisf, "__floatdisf")
     DEF(TOK___floatdidf, "__floatdidf")
#  ifndef TCC_ARM_VFP
     DEF(TOK___floatdixf, "__floatdixf")
     DEF(TOK___fixunssfsi, "__fixunssfsi")
     DEF(TOK___fixunsdfsi, "__fixunsdfsi")
     DEF(TOK___fixunsxfsi, "__fixunsxfsi")
     DEF(TOK___fixxfdi, "__fixxfdi")
#  endif
     DEF(TOK___fixsfdi, "__fixsfdi")
     DEF(TOK___fixdfdi, "__fixdfdi")
# endif
#endif

#if defined TCC_TARGET_C67
     DEF(TOK__divi, "_divi")
     DEF(TOK__divu, "_divu")
     DEF(TOK__divf, "_divf")
     DEF(TOK__divd, "_divd")
     DEF(TOK__remi, "_remi")
     DEF(TOK__remu, "_remu")
#endif

#if defined TCC_TARGET_I386
     DEF(TOK___fixsfdi, "__fixsfdi")
     DEF(TOK___fixdfdi, "__fixdfdi")
     DEF(TOK___fixxfdi, "__fixxfdi")
#endif
#if defined TCC_TARGET_X86_64
     DEF(TOK___fixxfdi, "__fixxfdi")
#endif

     DEF(TOK_alloca, "alloca")

#if defined TCC_TARGET_PE
     DEF(TOK___chkstk, "__chkstk")
     DEF(TOK___tls_index, "__tls_index")
#endif
#ifdef TCC_TARGET_ARM64
     DEF(TOK___arm64_clear_cache, "__arm64_clear_cache")
#endif
#ifdef TCC_TARGET_RISCV64
     DEF(TOK___riscv64_clear_cache, "__riscv64_clear_cache")
#endif
#if defined TCC_TARGET_ARM64 || defined TCC_TARGET_RISCV64
     DEF(TOK___addtf3, "__addtf3")
     DEF(TOK___subtf3, "__subtf3")
     DEF(TOK___multf3, "__multf3")
     DEF(TOK___divtf3, "__divtf3")
     DEF(TOK___extendsftf2, "__extendsftf2")
     DEF(TOK___extenddftf2, "__extenddftf2")
     DEF(TOK___trunctfsf2, "__trunctfsf2")
     DEF(TOK___trunctfdf2, "__trunctfdf2")
     DEF(TOK___negtf2, "__negtf2")
     DEF(TOK___fixtfsi, "__fixtfsi")
     DEF(TOK___fixtfdi, "__fixtfdi")
     DEF(TOK___fixunstfsi, "__fixunstfsi")
     DEF(TOK___fixunstfdi, "__fixunstfdi")
     DEF(TOK___floatsitf, "__floatsitf")
     DEF(TOK___floatditf, "__floatditf")
     DEF(TOK___floatunsitf, "__floatunsitf")
     DEF(TOK___floatunditf, "__floatunditf")
     DEF(TOK___eqtf2, "__eqtf2")
     DEF(TOK___netf2, "__netf2")
     DEF(TOK___lttf2, "__lttf2")
     DEF(TOK___letf2, "__letf2")
     DEF(TOK___gttf2, "__gttf2")
     DEF(TOK___getf2, "__getf2")
#endif

/* bound checking symbols */
#ifdef CONFIG_TCC_BCHECK
     DEF(TOK___bound_ptr_add, "__bound_ptr_add")
     DEF(TOK___bound_ptr_indir1, "__bound_ptr_indir1")
     DEF(TOK___bound_ptr_indir2, "__bound_ptr_indir2")
     DEF(TOK___bound_ptr_indir4, "__bound_ptr_indir4")
     DEF(TOK___bound_ptr_indir8, "__bound_ptr_indir8")
     DEF(TOK___bound_ptr_indir12, "__bound_ptr_indir12")
     DEF(TOK___bound_ptr_indir16, "__bound_ptr_indir16")
     DEF(TOK___bound_main_arg, "__bound_main_arg")
     DEF(TOK___bound_local_new, "__bound_local_new")
     DEF(TOK___bound_local_delete, "__bound_local_delete")
     DEF(TOK___bound_setjmp, "__bound_setjmp")
     DEF(TOK___bound_longjmp, "__bound_longjmp")
     DEF(TOK___bound_new_region, "__bound_new_region")
# ifdef TCC_TARGET_PE
#  ifdef TCC_TARGET_X86_64
     DEF(TOK___bound_alloca_nr, "__bound_alloca_nr")
#  endif
# else
     DEF(TOK_sigsetjmp, "sigsetjmp")
     DEF(TOK___sigsetjmp, "__sigsetjmp")
     DEF(TOK_siglongjmp, "siglongjmp")
# endif
     DEF(TOK_setjmp, "setjmp")
     DEF(TOK__setjmp, "_setjmp")
     DEF(TOK_longjmp, "longjmp")
#endif

/* -filc runtime. Always defined so the optional mode can call them. */
     DEF(TOK_filc_cover, "filc_cover")
     DEF(TOK_filc_uncover, "filc_uncover")
     DEF(TOK_filc_check, "filc_check")
     DEF(TOK_filc_check_func, "filc_check_func")
     DEF(TOK_filc_load_cap, "filc_load_cap")
     DEF(TOK_filc_store_cap, "filc_store_cap")
     DEF(TOK_filc_malloc, "filc_malloc")
     DEF(TOK_filc_calloc, "filc_calloc")
     DEF(TOK_filc_realloc, "filc_realloc")
     DEF(TOK_filc_free, "filc_free")
     DEF(TOK_filc_memcpy, "filc_memcpy")
     DEF(TOK_filc_memmove, "filc_memmove")
     DEF(TOK_filc_memset, "filc_memset")
     DEF(TOK_filc_strlen, "filc_strlen")
     DEF(TOK_filc_strcmp, "filc_strcmp")
     DEF(TOK_filc_strncmp, "filc_strncmp")
     DEF(TOK_filc_strchr, "filc_strchr")
     DEF(TOK_filc_strrchr, "filc_strrchr")
     DEF(TOK_filc_strstr, "filc_strstr")
     DEF(TOK_filc_strdup, "filc_strdup")
     DEF(TOK_filc_memchr, "filc_memchr")
     DEF(TOK_filc_call_push, "filc_call_push")
     DEF(TOK_filc_call_set, "filc_call_set")
     DEF(TOK_filc_call_get, "filc_call_get")
     DEF(TOK_filc_call_pop, "filc_call_pop")
     DEF(TOK_filc_set_ret, "filc_set_ret")
     DEF(TOK_filc_get_ret, "filc_get_ret")
     DEF(TOK_filc_set_ret_n, "filc_set_ret_n")
     DEF(TOK_filc_set_ret_at, "filc_set_ret_at")
     DEF(TOK_filc_get_ret_at, "filc_get_ret_at")
     DEF(TOK_filc_enter_main, "filc_enter_main")
     DEF(TOK_filc_track, "filc_track")
     DEF(TOK_filc_track_mark, "filc_track_mark")
     DEF(TOK_filc_untrack_to, "filc_untrack_to")
     DEF(TOK_filc_untrack_below, "filc_untrack_below")
     DEF(TOK_filc_label_set, "filc_label_set")
     DEF(TOK_filc_label_untrack, "filc_label_untrack")
     DEF(TOK_filc_label_clear, "filc_label_clear")
     DEF(TOK_filc_setjmp_anchor, "filc_setjmp_anchor")
     DEF(TOK_filc_longjmp, "filc_longjmp")
     DEF(TOK_filc__longjmp, "filc__longjmp")
     DEF(TOK_filc_siglongjmp, "filc_siglongjmp")
     DEF(TOK_filc_adapt, "filc_adapt")
     DEF(TOK_filc_store_ptr, "filc_store_ptr")
     DEF(TOK_filc_va_prepare, "filc_va_prepare")
     DEF(TOK_filc_va_take, "filc_va_take")
     DEF(TOK_filc_va_cursor, "filc_va_cursor")
     DEF(TOK_filc_va_start, "filc_va_start")
     DEF(TOK_filc_strcpy, "filc_strcpy")
     DEF(TOK_filc_strncpy, "filc_strncpy")
     DEF(TOK_filc_strcat, "filc_strcat")
     DEF(TOK_filc_strncat, "filc_strncat")
     DEF(TOK_filc_strnlen, "filc_strnlen")
     DEF(TOK_filc_memcmp, "filc_memcmp")
     DEF(TOK_filc_strndup, "filc_strndup")
     DEF(TOK_filc_stpcpy, "filc_stpcpy")
     DEF(TOK_filc_stpncpy, "filc_stpncpy")
     DEF(TOK_filc_strspn, "filc_strspn")
     DEF(TOK_filc_strcspn, "filc_strcspn")
     DEF(TOK_filc_strpbrk, "filc_strpbrk")
     DEF(TOK_filc_memccpy, "filc_memccpy")
     DEF(TOK_filc_explicit_bzero, "filc_explicit_bzero")
     DEF(TOK_filc_strtok, "filc_strtok")
     DEF(TOK_filc_strtok_r, "filc_strtok_r")
     DEF(TOK_filc_strsep, "filc_strsep")
     DEF(TOK_filc_read, "filc_read")
     DEF(TOK_filc_write, "filc_write")
     DEF(TOK_filc_pread, "filc_pread")
     DEF(TOK_filc_pwrite, "filc_pwrite")
     DEF(TOK_filc_fread, "filc_fread")
     DEF(TOK_filc_fwrite, "filc_fwrite")
     DEF(TOK_filc_fgets, "filc_fgets")
     DEF(TOK_filc_getenv, "filc_getenv")
     DEF(TOK_filc_secure_getenv, "filc_secure_getenv")
     DEF(TOK_filc_setenv, "filc_setenv")
     DEF(TOK_filc_unsetenv, "filc_unsetenv")
     DEF(TOK_filc_putenv, "filc_putenv")
     DEF(TOK_filc_open, "filc_open")
     DEF(TOK_filc_fopen, "filc_fopen")
     DEF(TOK_filc_freopen, "filc_freopen")
     DEF(TOK_filc_fdopen, "filc_fdopen")
     DEF(TOK_filc_puts, "filc_puts")
     DEF(TOK_filc_fputs, "filc_fputs")
     DEF(TOK_filc_fclose, "filc_fclose")
     DEF(TOK_filc_fflush, "filc_fflush")
     DEF(TOK_filc_fputc, "filc_fputc")
     DEF(TOK_filc_putchar, "filc_putchar")
     DEF(TOK_filc_perror, "filc_perror")
     DEF(TOK_filc_close, "filc_close")
     DEF(TOK_filc_lseek, "filc_lseek")
     DEF(TOK_filc_mmap, "filc_mmap")
     DEF(TOK_filc_munmap, "filc_munmap")
     DEF(TOK_filc_mprotect, "filc_mprotect")
     DEF(TOK_filc_fmt_begin, "filc_fmt_begin")
     DEF(TOK_filc_fmt_ptr, "filc_fmt_ptr")
     DEF(TOK_filc_fmt_skip, "filc_fmt_skip")
     DEF(TOK_filc_fmt_end, "filc_fmt_end")
     DEF(TOK_filc_scan_end, "filc_scan_end")
     DEF(TOK_filc_sprintf, "filc_sprintf")
     DEF(TOK_filc_snprintf, "filc_snprintf")
     DEF(TOK_filc_vsprintf, "filc_vsprintf")
     DEF(TOK_filc_vsnprintf, "filc_vsnprintf")
     DEF(TOK_filc_qsort, "filc_qsort")
     DEF(TOK_filc_bsearch, "filc_bsearch")
     DEF(TOK_filc_strcasecmp, "filc_strcasecmp")
     DEF(TOK_filc_strncasecmp, "filc_strncasecmp")
     DEF(TOK_filc_strcasestr, "filc_strcasestr")
     DEF(TOK_filc_memmem, "filc_memmem")
     DEF(TOK_filc_stat, "filc_stat")
     DEF(TOK_filc_lstat, "filc_lstat")
     DEF(TOK_filc_fstat, "filc_fstat")
     DEF(TOK_filc_strtol, "filc_strtol")
     DEF(TOK_filc_strtoll, "filc_strtoll")
     DEF(TOK_filc_strtoul, "filc_strtoul")
     DEF(TOK_filc_strtoull, "filc_strtoull")
     DEF(TOK_filc_strtof, "filc_strtof")
     DEF(TOK_filc_strtod, "filc_strtod")
     DEF(TOK_filc_strtold, "filc_strtold")
     DEF(TOK_filc_atoi, "filc_atoi")
     DEF(TOK_filc_atol, "filc_atol")
     DEF(TOK_filc_atoll, "filc_atoll")
     DEF(TOK_filc_atof, "filc_atof")
     DEF(TOK_filc_getcwd, "filc_getcwd")
     DEF(TOK_filc_dlopen, "filc_dlopen")
     DEF(TOK_filc_dlsym, "filc_dlsym")
     DEF(TOK_filc_dlclose, "filc_dlclose")
     DEF(TOK_filc_dlerror, "filc_dlerror")
     DEF(TOK_filc_vscan, "filc_vscan")
     DEF(TOK_filc_vprint, "filc_vprint")
     DEF(TOK_filc_atomic_load, "filc_atomic_load")
     DEF(TOK_filc_atomic_xchg, "filc_atomic_xchg")
     DEF(TOK_filc_atomic_cmpxchg, "filc_atomic_cmpxchg")

/*********************************************************************/
/* Tiny Assembler */
#define DEF_ASM(x) DEF(TOK_ASM_ ## x, #x)
#define DEF_ASMDIR(x) DEF(TOK_ASMDIR_ ## x, "." #x)
#define TOK_ASM_int TOK_INT

#define TOK_ASMDIR_FIRST TOK_ASMDIR_byte
#define TOK_ASMDIR_LAST TOK_ASMDIR_section

 DEF_ASMDIR(byte)       /* must be first directive */
 DEF_ASMDIR(word)
 DEF_ASMDIR(align)
 DEF_ASMDIR(balign)
 DEF_ASMDIR(p2align)
 DEF_ASMDIR(set)
 DEF_ASMDIR(skip)
 DEF_ASMDIR(space)
 DEF_ASMDIR(string)
 DEF_ASMDIR(asciz)
 DEF_ASMDIR(ascii)
 DEF_ASMDIR(file)
 DEF_ASMDIR(globl)
 DEF_ASMDIR(global)
 DEF_ASMDIR(weak)
 DEF_ASMDIR(hidden)
 DEF_ASMDIR(ident)
 DEF_ASMDIR(size)
 DEF_ASMDIR(type)
 DEF_ASMDIR(text)
 DEF_ASMDIR(data)
 DEF_ASMDIR(bss)
 DEF_ASMDIR(previous)
 DEF_ASMDIR(pushsection)
 DEF_ASMDIR(popsection)
 DEF_ASMDIR(fill)
 DEF_ASMDIR(rept)
 DEF_ASMDIR(endr)
 DEF_ASMDIR(org)
 DEF_ASMDIR(quad)
#if PTR_SIZE == 4
 DEF_ASMDIR(code16)
 DEF_ASMDIR(code32)
#else
 DEF_ASMDIR(code64)
#endif
#if defined(TCC_TARGET_RISCV64)
 DEF_ASMDIR(option)
#endif
 DEF_ASMDIR(short)
 DEF_ASMDIR(long)
 DEF_ASMDIR(int)
 DEF_ASMDIR(symver)
 DEF_ASMDIR(reloc)
 DEF_ASMDIR(section)    /* must be last directive */

#if defined TCC_TARGET_I386 || defined TCC_TARGET_X86_64
#include "i386-tok.h"
#endif

#if defined TCC_TARGET_ARM
#include "arm-tok.h"
#endif

#if defined TCC_TARGET_ARM64
#include "arm64-tok.h"
#endif

#if defined TCC_TARGET_RISCV64
#include "riscv64-tok.h"
#endif
