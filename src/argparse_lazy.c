#include "argparse_internal.h"

void argparse_option_set_lazy_default(struct argparse_option *option,
                                      argparse_lazy_default_fn fn,
                                      void *user_data)
{
    if (!option) return;
    option->lazy_default_fn = fn;
    option->lazy_default_data = user_data;
}
