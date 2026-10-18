#ifndef SHA256_H_
#define SHA256_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Maximum size of the computed hash value.
 */
#define SHA256_HEX_SIZE (64 + 1)

/**
 * @brief Compute the SHA-256 checksum of a memory region given a pointer and the size of that memory region. The output is a hexadecimal string of 65 characters. The last character will be the null-character.
 * @param source Source string to hash.
 * @param n_bytes The length — in bytes — of the source.
 * @param destination Destination pointer where the hashed string will be stored. This parametre can not be `NULL`.
 */
void sha256_hex(const void *source, size_t n_bytes, char *destination);

/**
 * @brief Encapsulation of a SHA-256 checksum.
 */
typedef struct
{
    uint32_t state[8];
    uint8_t buffer[64];
    uint64_t n_bits;
    uint8_t buffer_counter;
} sha256_t;

/**
 * @brief Initialize a given region of memory according to the SHA-256 algorithm.
 * @param algorithim Initialized region of memory returned through the function.
 */
void sha256_init(sha256_t *algorithm);

/**
 * @brief Append a sized region of memory to the given hash.
 * @param algorithim Hash to which to append.
 * @param data Data from which to append to the hash. This region will be cast to a `uint8_t`.
 * @param n_bytes The length — in bytes — of the data.
 */
void sha256_append(sha256_t *algorithm, const void *data, size_t n_bytes);

/**
 * @brief Finalize the hex value of the given algorithim.
 * @param algorithim Algoritim which to finalize.
 * @param destination Destination pointer where the hashed string will be stored. This parametre can not be `NULL`.
 */
void sha256_finalize_hex(sha256_t *algorithm, char *destination);

#endif // SHA256_H_


#ifdef SHA256_IMPLEMENTATION

/**
 * @brief Size of the `K` table.
 */
#define __K_WINDOW_SIZE 64

/**
 * @brief `K` table. This is how the algorithim works.
 */
static const uint32_t __k[__K_WINDOW_SIZE] =
{
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

/**
 * @brief Sample of 16-bit hex values to append to the hash.
 */
static const char __sample[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

/**
 * @brief Rotate bytes of the given `x` value to the right.
 * @param x Value of which to rotate.
 * @param n Step paramatre by which to rotate the given `x` value.
 * @returns The given `x` value rotated by `n` to the right.
 */
static inline uint32_t __rotate(uint32_t x, uint32_t n)
{
    return (x >> n) | (x << (32 - n));
}

/**
 * @brief Perform the first rotation on the given values.
 * @param e The first value to rotate.
 * @param f The second value to rotate.
 * @param g The third value to rotate.
 * @returns The result of each of the given values' rotation.
 */
static inline uint32_t __step1(uint32_t e, uint32_t f, uint32_t g)
{
    return (__rotate(e, 6) ^ __rotate(e, 11) ^ __rotate(e, 25)) + ((e & f) ^ ((~ e) & g));
}

/**
 * @brief Perform the second rotation on the given values.
 * @param a The first value to rotate.
 * @param b The second value to rotate.
 * @param c The third value to rotate.
 * @returns The result of each of the given values' rotation.
 */
static inline uint32_t __step2(uint32_t a, uint32_t b, uint32_t c)
{
    return (__rotate(a, 2) ^ __rotate(a, 13) ^ __rotate(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
}

/**
 * @brief Update the given window.
 * @param window Window of which to update.
 * @param index The size of the current window.
 * @param buffer The internal buffer to advance.
 * @param round The current iteration of updating the window.
 */
static inline void __update_window(uint32_t **window, size_t index, const uint8_t **buffer, size_t round)
{
    if (index < 16)
    {
        (*window)[round] =
                ((uint32_t)(*buffer)[0] << 24) |
                ((uint32_t)(*buffer)[1] << 16) |
                ((uint32_t)(*buffer)[2] <<  8) |
                ((uint32_t)(*buffer)[3]);
        (*buffer) += 4;
    }
    else
    {
        uint32_t a = (*window)[(round + 1) & 15];
        uint32_t b = (*window)[(round + 14) & 15];
        uint32_t s0 = (__rotate(a,  7) ^ __rotate(a, 18) ^ (a >>  3));
        uint32_t s1 = (__rotate(b, 17) ^ __rotate(b, 19) ^ (b >> 10));
        (*window)[round] += (*window)[(round + 9) & 15] + s0 + s1;
    }
}

/**
 * @brief Update each window within the internal algorithim.
 * @param window Internal window of the algorithim of which to update.
 * @param index The current iteration of updating the window.
 * @param buffer The internal buffer to advance.
 */
static inline void __update_windows(uint32_t *window, size_t index, const uint8_t *buffer)
{
    for (size_t round = 0; round < 16; round++)
    {
        __update_window(&window, index, &buffer, round);
    }
}

/**
 * @brief Process a block of memory according to the given algorithim.
 * @param algorithim Algorithim to process.
 */
static void __sha256_block(sha256_t *algorithm)
{
    // State of the program
    uint32_t *state = algorithm->state;

    uint32_t a = state[0];
    uint32_t b = state[1];
    uint32_t c = state[2];
    uint32_t d = state[3];
    uint32_t e = state[4];
    uint32_t f = state[5];
    uint32_t g = state[6];
    uint32_t h = state[7];

    uint32_t w[16] = {0};

    for (size_t i = 0; i < 64; i += 16)
    {
        __update_windows(w, i, algorithm->buffer);

        for (size_t j = 0; j < 16; j += 4)
        {
            // TODO: I don't like the way this is using a composition variable.
            uint32_t temp;
            temp = h + __step1(e, f, g) + __k[i + j + 0] + w[j + 0];
            h = temp + d;
            d = temp + __step2(a, b, c);
            temp = g + __step1(h, e, f) + __k[i + j + 1] + w[j + 1];
            g = temp + c;
            c = temp + __step2(d, a, b);
            temp = f + __step1(g, h, e) + __k[i + j + 2] + w[j + 2];
            f = temp + b;
            b = temp + __step2(c, d, a);
            temp = e + __step1(f, g, h) + __k[i + j + 3] + w[j + 3];
            e = temp + a;
            a = temp + __step2(b, c, d);
        }
    }
    // TODO: This can be a seperate function.
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

/**
 * @brief Initialize a given region of memory according to the SHA-256 algorithm.
 * @param algorithim Initialized region of memory returned through the function.
 */
void sha256_init(sha256_t *algorithm)
{
    algorithm->state[0] = 0x6a09e667;
    algorithm->state[1] = 0xbb67ae85;
    algorithm->state[2] = 0x3c6ef372;
    algorithm->state[3] = 0xa54ff53a;
    algorithm->state[4] = 0x510e527f;
    algorithm->state[5] = 0x9b05688c;
    algorithm->state[6] = 0x1f83d9ab;
    algorithm->state[7] = 0x5be0cd19;
    algorithm->n_bits = 0;
    algorithm->buffer_counter = 0;
}

/**
 * @brief Append a single byte to the algorithim.
 * @param algorithim Algorithim to which to append.
 * @param byte Single byte to append.
 */
static void __sha256_append_byte(sha256_t *algorithm, uint8_t byte)
{
    algorithm->buffer[algorithm->buffer_counter++] = byte;
    algorithm->n_bits += 8;

    if (algorithm->buffer_counter != 64) return;
    
    algorithm->buffer_counter = 0;
    __sha256_block(algorithm);
}

/**
 * @brief Append a sized region of memory to the given hash.
 * @param algorithim Hash to which to append.
 * @param data Data from which to append to the hash. This region will be cast to a `uint8_t`.
 * @param n_bytes The length — in bytes — of the data.
 */
void sha256_append(sha256_t *algorithm, const void *data, size_t n_bytes)
{
    const uint8_t *bytes = (const uint8_t*)data;

    for (size_t i = 0; i < n_bytes; i++)
    {
        __sha256_append_byte(algorithm, bytes[i]);
    }
}

/**
 * @brief Finalize the algorithim.
 * @param algorithim Algorithim to finalize.
 */
void sha256_finalize(sha256_t *algorithm)
{
    uint64_t n_bits = algorithm->n_bits;

    __sha256_append_byte(algorithm, 0x80);

    while (algorithm->buffer_counter != 56)
    {
        __sha256_append_byte(algorithm, 0);
    }

    for (int32_t i = 7; i >= 0; i--)
    {
        uint8_t byte = (n_bits >> 8 * i) & 0xff;
        __sha256_append_byte(algorithm, byte);
    }
}

/**
 * @brief Finalize the hex block of the algorithim.
 * @param state Current state of the alogorithim.
 * @param index Current iteration of the state.
 * @param destination Buffer to which to update.
 */
static inline void __finalize_block(const uint32_t *state, size_t index, char **destination)
{
    for (int32_t j = 7; j >= 0; j--)
    {
        uint8_t nibble = ((state)[index] >> j * 4) & 0xf;
        (*(*destination)++) = __sample[nibble];
    }
}

/**
 * @brief Finalize the hex value of the given algorithim.
 * @param algorithim Algoritim which to finalize.
 * @param destination Destination pointer where the hashed string will be stored. This parametre can not be `NULL`.
 */
void sha256_finalize_hex(sha256_t *algorithm, char *destination)
{
    sha256_finalize(algorithm);

    for (size_t i = 0; i < 8; i++)
    {
        __finalize_block(algorithm->state, i, &destination);
    }

    *destination = '\0';
}

/**
 * @brief Compute the SHA-256 checksum of a memory region given a pointer and the size of that memory region. The output is a hexadecimal string of 65 characters. The last character will be the null-character.
 * @param source Source string to hash.
 * @param n_bytes The length — in bytes — of the source.
 * @param destination Destination pointer where the hashed string will be stored. This parametre can not be `NULL`.
 */
void sha256_hex(const void *source, size_t n_bytes, char *destination)
{
    sha256_t algorithm = {0};
    sha256_init(&algorithm);
    sha256_append(&algorithm, source, n_bytes);
    sha256_finalize_hex(&algorithm, destination);
}

#endif // SHA256_IMPLEMENTATION