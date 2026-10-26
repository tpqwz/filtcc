/* va_list.c - tinycc support for va_list on X86_64 */

#if defined __x86_64__

/* Avoid include files, they may not be available when cross compiling */
extern void abort(void);

/* This should be in sync with our include/stdarg.h */
enum __va_arg_type {
    __va_gen_reg, __va_float_reg, __va_stack
};

/* GCC compatible definition of va_list. */
/*predefined by TCC (tcc_predefs.h):
typedef struct {
    unsigned int gp_offset;
    unsigned int fp_offset;
    union {
        unsigned int overflow_offset;
        char *overflow_arg_area;
    };
    char *reg_save_area;
} __builtin_va_list[1];
*/

extern void *memcpy(void *dest, const void *src, unsigned long n);

void *__va_arg(__builtin_va_list ap,
               int arg_type,
               int size, int align)
{
    size = (size + 7) & ~7;
    align = (align + 7) & ~7;
    switch ((enum __va_arg_type)arg_type) {
    case __va_gen_reg:
        if (ap->gp_offset + size <= 48) {
            ap->gp_offset += size;
            return ap->reg_save_area + ap->gp_offset - size;
        }
        goto use_overflow_area;

    case __va_float_reg:
        if (ap->fp_offset < 128 + 48) {
            ap->fp_offset += 16;
            if (size == 8)
                return ap->reg_save_area + ap->fp_offset - 16;
            if (ap->fp_offset < 128 + 48) {
                memcpy(ap->reg_save_area + ap->fp_offset - 8,
                       ap->reg_save_area + ap->fp_offset, 8);
                ap->fp_offset += 16;
                return ap->reg_save_area + ap->fp_offset - 32;
            }
        }
        goto use_overflow_area;

    case __va_stack:
    use_overflow_area:
        {
            /* Same integer result as aligning through a cast. The delta
             * is computed as an integer so a capability on the cursor
             * survives the store. */
            char *p;
            unsigned long u, a;
            p = ap->overflow_arg_area + size;
            a = (unsigned long)align;
            u = (unsigned long)p;
            u = (u + a - 1) & -a;
            p += u - (unsigned long)p;
            ap->overflow_arg_area = p;
            return p - size;
        }

    default: /* should never happen */
        abort();
        return 0;
    }
}
#endif
