#include <stdint.h>

typedef uint32_t fc_unsigned_int_t;
typedef uint64_t name_t;
typedef uint64_t symbol_t;

typedef struct transaction_header_t {
    uint32_t expiration;
    uint16_t ref_block_num;
    uint32_t ref_block_prefix;
    fc_unsigned_int_t max_net_usage_words;
    uint8_t max_cpu_usage_ms;
    fc_unsigned_int_t delay_sec;
} transaction_header_t;

typedef struct action_t {
    name_t account;
    name_t name;
    // vector<permission_level> autorization;
    // vector<char> bytes;
} action_t;

typedef struct permisssion_level_t {
    name_t actor;
    name_t permission;
} permisssion_level_t;

typedef struct asset_t {
    int64_t amount;
    symbol_t symbol;
} asset_t;