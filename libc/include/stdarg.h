#ifndef _STDARG_H
#define _STDARG_H

// Define va_list as a pointer to an argument (assuming 32-bit system)
typedef char* va_list;

// va_start macro - Initialize the va_list to the first argument
#define va_start(ap, param) (ap = (va_list)&param + sizeof(param))

// va_arg macro - Retrieve the next argument from the va_list
#define va_arg(ap, type)   (*(type *)(ap += sizeof(type) - sizeof(char*)))

#define va_end(ap)   // No-op for this simple version

#endif // _STDARG_H
