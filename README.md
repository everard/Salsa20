SALSA20
=======

This repository contains C++11 implementation of the stream cypher Salsa20 (http://www.ecrypt.eu.org/stream/salsa20pf.html).
Salsa20 home page: http://cr.yp.to/snuffle.html

HOW TO USE
==========

1. Add Salsa20.h and Salsa20.cpp files to your project.
2. In your C++ code you should initialize cypher with key and IV:

        salsa20::Cypher cypher(key);
        cypher.setIv(iv);

3. Now you can use processBlocks member function to encrypt or decrypt blocks (size of the block equals to 64 bytes).
   If you would like to process number of bytes not multiple of block size, then use processBytes member function.

        cypher.processBlocks(input, output, numBlocks);
        cypher.processBytes(input, output, numBytes);

   Note that processBytes may only be used once (typically after consecutive calls to processBlocks member function).

4. You can also use cypher to generate key stream (which is basically a random 64 byte long value):

        std::uint8_t keyStream[salsa20::Cypher::BLOCK_SIZE];
        cypher.generateKeyStream(keyStream);

COMMAND LINE INTRFACE
=====================
Another option is to build command line utility which can encrypt and decrypt files:

        make
        sudo make install

And then use it like this:
        salsa20 -h

LICENSE
=======
Copyright (c) 2014 Nezametdinov E. Ildus

This software is licensed under the MIT License (see LICENSE.txt).
