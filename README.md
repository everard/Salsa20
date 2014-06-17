Salsa20
=======

This is C++11 implementation of the stream cypher Salsa20 based on the reference implementation: http://www.ecrypt.eu.org/stream/salsa20pf.html
Salsa20 home page: http://cr.yp.to/snuffle.html

How to use
==========

1. Add Salsa20.h and Salsa20.cpp files to your project.
2. Initialize cypher:

    salsa::Cypher cypher(key);
    cypher.setIv(iv);

3. Use processBlocks member function to encrypt or decrypt blocks (size of the block equals to 64 bits).
   If you would like to process number of bytes not multiple of block size, then use processBytes member function.

    cypher.processBlocks(input, output, numBlocks);
    cypher.processBytes(input, output, numBytes);

Note that processBytes may be used only once (typically after processBlocks calls).

LICENSE
=======
Copyright (c) 2014 Nezametdinov E. Ildus

This software is licensed under the MIT License (see LICENSE.txt).
