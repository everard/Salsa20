// Copyright (c) 2014 Nezametdinov E. Ildus
// See LICENSE.TXT for licensing details

#ifndef SALSA_20_H
#define SALSA_20_H

#include <cstdint>
#include <cstring>

namespace salsa
{

        using std::size_t;
        using std::int32_t;
        using std::uint8_t;
        using std::uint32_t;

        /**
         * Represents Salsa20 cypher. Supports 256-bit keys and little-endian architecture.
         */
        class Cypher
        {
        public:
                /// Helper constants
                enum
                {
                        VECTOR_SIZE = 16,
                        BLOCK_SIZE = 64,
                        KEY_SIZE = 32,
                        IV_SIZE = 8
                };

                /**
                 * \brief Constructs cypher with given key.
                 * \param[in] key 256-bit key
                 */
                Cypher(const uint8_t* key = nullptr);
                Cypher(const Cypher&) = default;
                Cypher(Cypher&&) = default;
                ~Cypher() = default;
                Cypher& operator =(const Cypher&) = default;
                Cypher& operator =(Cypher&&) = default;

                /**
                 * \brief Sets key.
                 * \param[in] key 256-bit key
                 */
                void setKey(const uint8_t* key);

                /**
                 * \brief Sets IV.
                 * \param[in] iv 64-bit IV
                 */
                void setIv(const uint8_t* iv);

                /**
                 * \brief Generates key stream.
                 * \param[out] output generated key stream
                 */
                void generateKeyStream(uint8_t output[BLOCK_SIZE]);

                /**
                 * \brief Processes blocks.
                 * \param[in] input input
                 * \param[out] output output
                 * \param[in] numBlocks number of blocks
                 */
                void processBlocks(uint8_t* input, uint8_t* output, size_t numBlocks);

                /**
                 * \brief Processes bytes.
                 *
                 * This function should be used carefully. If number of bytes is not multiple of
                 * block size, then next call to the processBlocks function will be invalid.
                 * Normally this function should be used once at the end of encryption or
                 * decryption.
                 * \param[in] input input
                 * \param[out] output output
                 * \param[in] numBytes number of bytes
                 */
                void processBytes(uint8_t* input, uint8_t* output, size_t numBytes);

        private:
                uint32_t vector_[VECTOR_SIZE];

                /**
                 * \brief Rotates value.
                 * \param[in] value value
                 * \param[in] numBits number of bits to rotate
                 * \return result of the rotation
                 */
                uint32_t rotate(uint32_t value, uint32_t numBits);

        };

}

#endif
