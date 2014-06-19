// Copyright (c) 2014 Nezametdinov E. Ildus
// See LICENSE.TXT for licensing details

#include "Salsa20.h"

namespace salsa20
{

        Cypher::Cypher(const uint8_t* key)
        {
                std::memset(vector_, 0, sizeof(vector_));
                setKey(key);
        }

        //---------------------------------------------------------------------------------
        void Cypher::setKey(const uint8_t* key)
        {
                static const char constants[] = "expand 32-byte k";

                if(key == nullptr)
                        return;

                std::memcpy(&vector_[1 ], &key[0 ], KEY_SIZE / 2);
                std::memcpy(&vector_[11], &key[16], KEY_SIZE / 2);

                std::memcpy(&vector_[0 ], &constants[0 ], sizeof(uint32_t));
                std::memcpy(&vector_[5 ], &constants[4 ], sizeof(uint32_t));
                std::memcpy(&vector_[10], &constants[8 ], sizeof(uint32_t));
                std::memcpy(&vector_[15], &constants[12], sizeof(uint32_t));

                std::memset(&vector_[6], 0, 4 * sizeof(uint32_t));
        }

        //---------------------------------------------------------------------------------
        void Cypher::setIv(const uint8_t* iv)
        {
                if(iv == nullptr)
                        return;

                std::memcpy(&vector_[6], iv, IV_SIZE);
                std::memset(&vector_[8], 0, 2 * sizeof(uint32_t));
        }

        //---------------------------------------------------------------------------------
        void Cypher::generateKeyStream(uint8_t output[BLOCK_SIZE])
        {
                uint32_t x[VECTOR_SIZE];
                std::memcpy(x, vector_, sizeof(vector_));

                for(int32_t i = 20; i > 0; i -= 2)
                {
                        x[4 ] ^= rotate(static_cast<uint32_t>(x[0 ] + x[12]),  7);
                        x[8 ] ^= rotate(static_cast<uint32_t>(x[4 ] + x[0 ]),  9);
                        x[12] ^= rotate(static_cast<uint32_t>(x[8 ] + x[4 ]), 13);
                        x[0 ] ^= rotate(static_cast<uint32_t>(x[12] + x[8 ]), 18);
                        x[9 ] ^= rotate(static_cast<uint32_t>(x[5 ] + x[1 ]),  7);
                        x[13] ^= rotate(static_cast<uint32_t>(x[9 ] + x[5 ]),  9);
                        x[1 ] ^= rotate(static_cast<uint32_t>(x[13] + x[9 ]), 13);
                        x[5 ] ^= rotate(static_cast<uint32_t>(x[1 ] + x[13]), 18);
                        x[14] ^= rotate(static_cast<uint32_t>(x[10] + x[6 ]),  7);
                        x[2 ] ^= rotate(static_cast<uint32_t>(x[14] + x[10]),  9);
                        x[6 ] ^= rotate(static_cast<uint32_t>(x[2 ] + x[14]), 13);
                        x[10] ^= rotate(static_cast<uint32_t>(x[6 ] + x[2 ]), 18);
                        x[3 ] ^= rotate(static_cast<uint32_t>(x[15] + x[11]),  7);
                        x[7 ] ^= rotate(static_cast<uint32_t>(x[3 ] + x[15]),  9);
                        x[11] ^= rotate(static_cast<uint32_t>(x[7 ] + x[3 ]), 13);
                        x[15] ^= rotate(static_cast<uint32_t>(x[11] + x[7 ]), 18);
                        x[1 ] ^= rotate(static_cast<uint32_t>(x[0 ] + x[3 ]),  7);
                        x[2 ] ^= rotate(static_cast<uint32_t>(x[1 ] + x[0 ]),  9);
                        x[3 ] ^= rotate(static_cast<uint32_t>(x[2 ] + x[1 ]), 13);
                        x[0 ] ^= rotate(static_cast<uint32_t>(x[3 ] + x[2 ]), 18);
                        x[6 ] ^= rotate(static_cast<uint32_t>(x[5 ] + x[4 ]),  7);
                        x[7 ] ^= rotate(static_cast<uint32_t>(x[6 ] + x[5 ]),  9);
                        x[4 ] ^= rotate(static_cast<uint32_t>(x[7 ] + x[6 ]), 13);
                        x[5 ] ^= rotate(static_cast<uint32_t>(x[4 ] + x[7 ]), 18);
                        x[11] ^= rotate(static_cast<uint32_t>(x[10] + x[9 ]),  7);
                        x[8 ] ^= rotate(static_cast<uint32_t>(x[11] + x[10]),  9);
                        x[9 ] ^= rotate(static_cast<uint32_t>(x[8 ] + x[11]), 13);
                        x[10] ^= rotate(static_cast<uint32_t>(x[9 ] + x[8 ]), 18);
                        x[12] ^= rotate(static_cast<uint32_t>(x[15] + x[14]),  7);
                        x[13] ^= rotate(static_cast<uint32_t>(x[12] + x[15]),  9);
                        x[14] ^= rotate(static_cast<uint32_t>(x[13] + x[12]), 13);
                        x[15] ^= rotate(static_cast<uint32_t>(x[14] + x[13]), 18);
                }

                for(uint32_t i = 0; i < VECTOR_SIZE; ++i)
                {
                        x[i] += vector_[i];
                        std::memcpy(&output[4 * i], &x[i], sizeof(uint32_t));
                }

                ++vector_[8];
                vector_[9] += vector_[8] == 0 ? 1 : 0;
        }

        //---------------------------------------------------------------------------------
        void Cypher::processBlocks(const uint8_t* input, uint8_t* output, size_t numBlocks)
        {
                uint8_t keyStream[BLOCK_SIZE];

                for(size_t i = 0; i < numBlocks; ++i)
                {
                        generateKeyStream(keyStream);

                        for(uint32_t j = 0; j < BLOCK_SIZE; ++j)
                                *(output++) = keyStream[j] ^ *(input++);
                }
        }

        //---------------------------------------------------------------------------------
        void Cypher::processBytes(const uint8_t* input, uint8_t* output, size_t numBytes)
        {
                const size_t blockSize = BLOCK_SIZE;
                uint8_t keyStream[BLOCK_SIZE];
                size_t numBytesToProcess;

                while(numBytes != 0)
                {
                        generateKeyStream(keyStream);
                        numBytesToProcess = numBytes >= blockSize ? blockSize : numBytes;

                        for(size_t i = 0; i < numBytesToProcess; ++i, --numBytes)
                                *(output++) = keyStream[i] ^ *(input++);
                }
        }

        //---------------------------------------------------------------------------------
        inline uint32_t Cypher::rotate(uint32_t value, uint32_t numBits)
        {
                return (value << numBits) | (value >> (32 - numBits));
        }

}
