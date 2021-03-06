#ifndef XXHASH_H
#define XXHASH_H

/*
   xxHash - Extremely Fast Hash algorithm
   Header File
   Copyright (C) 2012-2015, Yann Collet.

   BSD 2-Clause License (http://www.opensource.org/licenses/bsd-license.php)

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:

       * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above
   copyright notice, this list of conditions and the following disclaimer
   in the documentation and/or other materials provided with the
   distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   You can contact the author at :
   - xxHash source repository : https://github.com/Cyan4973/xxHash
*/

/* Notice extracted from xxHash homepage :

xxHash is an extremely fast Hash algorithm, running at RAM speed limits.
It also successfully passes all tests from the SMHasher suite.

Comparison (single thread, Windows Seven 32 bits, using SMHasher on a Core 2 Duo @3GHz)

Name            Speed       Q.Score   Author
xxHash          5.4 GB/s     10
CrapWow         3.2 GB/s      2       Andrew
MumurHash 3a    2.7 GB/s     10       Austin Appleby
SpookyHash      2.0 GB/s     10       Bob Jenkins
SBox            1.4 GB/s      9       Bret Mulvey
Lookup3         1.2 GB/s      9       Bob Jenkins
SuperFastHash   1.2 GB/s      1       Paul Hsieh
CityHash64      1.05 GB/s    10       Pike & Alakuijala
FNV             0.55 GB/s     5       Fowler, Noll, Vo
CRC32           0.43 GB/s     9
MD5-32          0.33 GB/s    10       Ronald L. Rivest
SHA1-32         0.28 GB/s    10

Q.Score is a measure of quality of the hash function.
It depends on successfully passing SMHasher test set.
10 is a perfect score.

*/

#if defined (__cplusplus)
extern "C" {
#endif


/*****************************
*  Definitions
*****************************/
#include "gpuarray/config.h"
typedef enum { XXH_OK=0, XXH_ERROR } XXH_errorcode;


/*****************************
*  Namespace Emulation
*****************************/
/* Motivations :

If you need to include xxHash into your library,
but wish to avoid xxHash symbols to be present on your library interface
in an effort to avoid potential name collision if another library also includes xxHash,

you can use XXH_NAMESPACE, which will automatically prefix any symbol from xxHash
with the value of XXH_NAMESPACE (so avoid to keep it NULL, and avoid numeric values).

Note that no change is required within the calling program :
it can still call xxHash functions using their regular name.
They will be automatically translated by this header.
*/
#ifdef XXH_NAMESPACE
#  define XXH_CAT(A,B) A##B
#  define XXH_NAME2(A,B) XXH_CAT(A,B)
#  define XXH32 XXH_NAME2(XXH_NAMESPACE, XXH32)
#  define XXH32_createState XXH_NAME2(XXH_NAMESPACE, XXH32_createState)
#  define XXH32_freeState XXH_NAME2(XXH_NAMESPACE, XXH32_freeState)
#  define XXH32_reset XXH_NAME2(XXH_NAMESPACE, XXH32_reset)
#  define XXH32_update XXH_NAME2(XXH_NAMESPACE, XXH32_update)
#  define XXH32_digest XXH_NAME2(XXH_NAMESPACE, XXH32_digest)
#endif


/*****************************
*  Simple Hash Functions
*****************************/

GPUARRAY_LOCAL unsigned int XXH32 (const void* input, size_t length, unsigned seed);

/*
XXH32() :
    Calculate the 32-bits hash of sequence "length" bytes stored at memory address "input".
    The memory between input & input+length must be valid (allocated and read-accessible).
    "seed" can be used to alter the result predictably.
    This function successfully passes all SMHasher tests.
    Speed on Core 2 Duo @ 3 GHz (single thread, SMHasher benchmark) : 5.4 GB/s
*/



/*****************************
*  Advanced Hash Functions
*****************************/
typedef struct { long long ll[ 6]; } XXH32_state_t;

/*
These structures allow static allocation of XXH states.
States must then be initialized using XXH32_reset() before first use.
*/

GPUARRAY_LOCAL XXH_errorcode XXH32_reset  (XXH32_state_t* statePtr, unsigned seed);
GPUARRAY_LOCAL XXH_errorcode XXH32_update (XXH32_state_t* statePtr, const void* input, size_t length);
GPUARRAY_LOCAL unsigned int  XXH32_digest (const XXH32_state_t* statePtr);

/*
These functions calculate the xxHash of an input provided in multiple smaller packets,
as opposed to an input provided as a single block.

XXH state space must first be allocated, using either static or dynamic method provided above.

Start a new hash by initializing state with a seed, using XXHnn_reset().

Then, feed the hash state by calling XXHnn_update() as many times as necessary.
Obviously, input must be valid, meaning allocated and read accessible.
The function returns an error code, with 0 meaning OK, and any other value meaning there is an error.

Finally, you can produce a hash anytime, by using XXHnn_digest().
This function returns the final nn-bits hash.
You can nonetheless continue feeding the hash state with more input,
and therefore get some new hashes, by calling again XXHnn_digest().

When you are done, don't forget to free XXH state space, using typically XXHnn_freeState().
*/


#if defined (__cplusplus)
}
#endif

#endif
