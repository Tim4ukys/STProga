#ifndef __BASE_BIT_COPY_H__
#define __BASE_BIT_COPY_H__

#include <stdint.h> // size_t

/**
* \param dst битовая цепочка, куда будет копироваться
* \param src битовая цепочка, от куда будет копироваться
* \param dst_ofs смещение в конечной переменной 
* \param src_ofs смещение от куда начинать запись
* \param bit_count кол-во копируемых битов
*/
void bitcpy(unsigned char* dst, size_t dst_ofs,
            unsigned char* src, size_t src_ofs, size_t bit_count);

#endif
