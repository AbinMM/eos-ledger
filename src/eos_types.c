#include "eos_types.h"
#include "os.h"
#include <stdbool.h>

static const char* charmap = ".12345abcdefghijklmnopqrstuvwxyz";

uint8_t name_to_string(name_t value, char *out, uint32_t size) {
    if (size > 13) {
        return 0;
    }

    uint32_t i = 0;
    uint32_t actual_size = 13;
    char tmp[13] = {'.'};

    for (i = 0; i <= 12; ++i) {
        char c = charmap[value & (i == 0 ? 0x0f : 0x1f)];
        tmp[12 - i] = c;
        value >>= (i == 0 ? 4 : 5);
    }

    while (tmp[actual_size - 1] == '.' && --actual_size != 0);

    os_memmove(out, tmp, actual_size);
    return actual_size;
}

bool is_valid_symbol(symbol_t sym) {
    sym >>= 8;
    for( int i = 0; i < 7; ++i ) {
        char c = (char)(sym & 0xff);
        if( !('A' <= c && c <= 'Z')  ) return false;
        sym >>= 8;
        if( !(sym & 0xff) ) {
            do {
                sym >>= 8;
                if( (sym & 0xff) ) return false;
                ++i;
            } while( i < 7 );
        }
    }
    return true;
}

uint32_t symbol_length(symbol_t tmp) {
    tmp >>= 8; /// skip precision
    uint32_t length = 0;
    while( tmp & 0xff && length <= 7) {
        ++length;
        tmp >>= 8;
    }

    return length;
}

uint64_t symbol_precision(symbol_t sym) {
    return sym & 0xff;
}

uint8_t symbol_to_string(symbol_t sym, char *out, uint32_t size) {
    sym >>= 8;

    if (size < 8) {
        return 0;
    }

    uint8_t i = 0;
    char tmp[8] = {0};

    for (i = 0; i < 7; ++i) {
        char c = (char)(sym & 0xff);
        if (!c) break;
        tmp[i] = c;
        sym >>= 8;
    }

    os_memmove(out, tmp, i);
    return i;
}

uint8_t asset_to_string(asset_t *asset, char *out, uint32_t size) {
    if (asset == NULL) {
        return 0;
    }

    int64_t p = (int64_t)symbol_precision(asset->symbol);
    int64_t p10 = 1;
    while (p > 0) {
        p10 *= 10; --p;
    }
    p = (int64_t)symbol_precision(asset->symbol);

    char fraction[p+1];
    fraction[p] = '\0';
    int64_t change = asset->amount % p10;

    for (int64_t i = p - 1; i >= 0; --i) {
        fraction[i] = (change % 10) + '0';
        change /= 10;
    }

    char symbol[9] = {0};
    symbol_to_string(asset->symbol, symbol, 8);

    char tmp[64] = {0};
    snprintf(tmp, 64, "%lld.%s %s", asset->amount/p10, fraction, symbol);

    uint8_t actual_size = 0;
    while(actual_size < sizeof(tmp) && tmp[actual_size] != 0) {
        actual_size++;
    } 

    os_memmove(out, tmp, actual_size);

    return actual_size;
}

uint8_t pack_fc_unsigned_int(fc_unsigned_int_t value, char *out) {
    uint8_t i = 0;
    do {
        uint8_t b = (uint8_t)(value & 0x7f);
        value >>= 7;
        b |= ((value > 0) << 7);
        *(out + i++) = b;
    } while (value);

    return i;
}