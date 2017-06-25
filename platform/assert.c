#include "assert.h"
#include "print_log.h"

void ecode_assert_failed(e_uint8_t *file, e_uint32_t line)
{
    print_log("Wrong parameters value: file %s on line %d\r\n", file, line);
    while(1);
}