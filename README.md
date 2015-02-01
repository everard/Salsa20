SALSA20
=======

This repository contains C++11 implementation of the stream cypher Salsa20 (http://www.ecrypt.eu.org/stream/salsa20pf.html).
Salsa20 home page: http://cr.yp.to/snuffle.html

HOW TO USE
==========

1. Add Salsa20.h and Salsa20.inl files to your project.
2. In your C++ code initialize cypher with secret key and IV:

        ucstk::Salsa20 salsa20(key);
        salsa20.setIv(iv);

3. Now you can use Salsa20::processBlocks to encrypt or decrypt blocks (size of the block equals to 64 bytes).
   If you would like to process number of bytes not multiple of block size, then use Salsa20::processBytes.

        salsa20.processBlocks(input, output, numBlocks);
        salsa20.processBytes(input, output, numBytes);

   Note that Salsa20::processBytes may only be used once (typically after consecutive calls to Salsa20::processBlocks).

4. You can also use cypher to generate key stream (which is a random sequence of 64 bytes):

        std::uint8_t keyStream[ucstk::Salsa20::BLOCK_SIZE];
        salsa20.generateKeyStream(keyStream);

COMMAND LINE INTERFACE
======================
This repository also contains command line utility which can encrypt and decrypt files. To build it you should run the following
commands in your shell:

        make
        sudo make install

Now you can view help on how to use this utility by typing the following command in your shell:

        salsa20 -h

LICENSE
=======
Copyright (c) 2014 Nezametdinov E. Ildus

This software is licensed under the MIT License (see LICENSE.txt).
