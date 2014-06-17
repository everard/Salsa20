// Copyright (c) 2014 Nezametdinov E. Ildus
// See LICENSE.TXT for licensing details

#include "Salsa20.h"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace salsa;

/**
 * Represents program.
 */
class Program
{
public:
        Program(): inputFileName_(), outputFileName_()
        {
                std::memset(key_, 0, sizeof(key_));
        }
        Program(const Program&) = delete;
        Program(Program&&) = delete;
        ~Program() {}
        Program& operator =(const Program&) = delete;
        Program& operator =(Program&&) = delete;

        /**
         * \brief Reads parameters from command line and validates them.
         * \param[in] argc number of command line arguments passed
         * \param[in] argv array of command line arguments
         * \return true on success
         */
        bool initialize(int argc, char* argv[])
        {
                std::string key;
                auto lastArgumentIndex = argc - 1;

                for(int i = 0; i < argc; ++i)
                {
                        std::string parameter = argv[i];

                        if(parameter == "-i" && i < lastArgumentIndex)
                        {
                                inputFileName_ = argv[++i];
                                continue;
                        }

                        if(parameter == "-o" && i < lastArgumentIndex)
                        {
                                outputFileName_ = argv[++i];
                                continue;
                        }

                        if(parameter == "-k" && i < lastArgumentIndex)
                        {
                                key = argv[++i];
                                continue;
                        }
                }

                if(inputFileName_.empty())
                {
                        std::cout << "error: input file name should be specified with -i" << std::endl;
                        return false;
                }

                if(outputFileName_.empty())
                {
                        std::cout << "error: output file name should be specified with -o" << std::endl;
                        return false;
                }

                if(inputFileName_ == outputFileName_)
                {
                        std::cout << "error: input and output files should be distinct" << std::endl;
                        return false;
                }

                if(key.empty())
                {
                        std::cout << "error: key should be specified with -k" << std::endl;
                        return false;
                }

                if(!ReadKeyFromString(key))
                {
                        std::cout << "error: key should be passed as HEX value (for example, 00112233...)";
                        std::cout << " and be " << sizeof(key_) << " bytes long" << std::endl;
                        return false;
                }

                return true;
        }

        /**
         * \brief Encrypts or decrypts the file.
         * \return true on success
         */
        bool execute()
        {
                std::ifstream inputStream(inputFileName_, std::ios_base::binary);
                if(!inputStream)
                {
                        std::cout << "error: could not open input file" << std::endl;
                        return false;
                }

                std::ofstream outputStream(outputFileName_, std::ios_base::binary);
                if(!outputStream)
                {
                        std::cout << "error: could not create output file" << std::endl;
                        return false;
                }

                const size_t chunkSize = NUM_OF_BLOCKS_PER_CHUNK * Cypher::BLOCK_SIZE;
                uint8_t chunk[chunkSize];

                // determine size of the file
                inputStream.seekg(0, std::ios_base::end);
                size_t fileSize = inputStream.tellg();
                inputStream.seekg(0, std::ios_base::beg);

                // compute number of chunks and size of the remainder
                size_t numChunks = fileSize / chunkSize;
                size_t remainderSize = fileSize % chunkSize;

                // process file
                salsa::Cypher cypher(key_);
                cypher.setIv(&key_[IV_OFFSET]);
                std::cout << "processing file \"" << inputFileName_ << '"' << std::endl;

                for(size_t i = 0; i < numChunks; ++i)
                {
                        inputStream.read(reinterpret_cast<char*>(chunk), sizeof(chunk));
                        cypher.processBlocks(chunk, chunk, NUM_OF_BLOCKS_PER_CHUNK);
                        outputStream.write(reinterpret_cast<const char*>(chunk), sizeof(chunk));

                        float percentage = 100.0f * static_cast<float>(i + 1) / static_cast<float>(numChunks);
                        printf("[%3.2f]\r", percentage);
                }

                if(remainderSize != 0)
                {
                        inputStream.read(reinterpret_cast<char*>(chunk), remainderSize);
                        cypher.processBytes(chunk, chunk, remainderSize);
                        outputStream.write(reinterpret_cast<const char*>(chunk), remainderSize);
                }

                std::cout << std::endl << "done" << std::endl;
                return true;
        }

private:
        /// Helper constants
        enum
        {
                NUM_OF_BLOCKS_PER_CHUNK = 4096,
                IV_OFFSET = salsa::Cypher::KEY_SIZE,
                KEY_SIZE  = salsa::Cypher::KEY_SIZE + salsa::Cypher::IV_SIZE
        };

        std::string inputFileName_, outputFileName_;
        uint8_t key_[KEY_SIZE];

        /**
         * \brief Reads byte from string.
         * \param[in] string string
         * \param[out] byte byte
         * \return true on success
         */
        bool readByte(const char* string, uint8_t& byte)
        {
                byte = 0;

                for(size_t i = 0; i < 2; ++i)
                {
                        uint8_t value = 0;
                        char c = string[i];

                        if(c >= '0' && c <= '9')
                                value = c - '0';
                        else if(c >= 'A' && c <= 'F')
                                value = c - 'A';
                        else
                                return false;

                        byte |= (value << (i * 4));
                }

                return true;
        }

        /**
         * \brief Reads key from string.
         * \param[in] string string
         * \return true on success
         */
        bool ReadKeyFromString(const std::string& string)
        {
                if(string.length() != 2 * KEY_SIZE)
                        return false;

                for(size_t i = 0; i < string.length(); i += 2)
                {
                        if(!readByte(&string[i], key_[i / 2]))
                                return false;
                }

                return true;
        }

};

// Entry point
int main(int argc, char* argv[])
{
        Program program;

        if(!program.initialize(argc, argv))
                return 1;

        if(!program.execute())
                return 2;

        return 0;
}
