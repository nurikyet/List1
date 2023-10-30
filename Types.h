#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#define PRINTFELEM "%d"
/**
 * @brief class type for stack
 *
 * this class stores the data type for working with the stack
 */
typedef int elem_t;

typedef int (*dump_f)(FILE*, const void*, const char*, const char*, const int);
#endif // TYPES_H_INCLUDED
