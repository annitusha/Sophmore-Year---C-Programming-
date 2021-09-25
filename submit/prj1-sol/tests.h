#ifndef TEST_H_
#define TEST_H_

#ifdef DO_TESTS

#if BYTE_SIZE == 2
typedef unsigned short Byte;
#endif

//expose static functions for testing
#define static
#define inline

/** Return mask for a Byte with bit at bitIndex set to 1, all other
 *  bits set to 0.  Note that bitIndex == 0 represents the MSB,
 *  bitIndex == 1 represents the next significant bit and so on.
 */
unsigned byteBitMask(unsigned bitIndex);

/** Given a power-of-2 powerOf2, return log2(powerOf2) */
unsigned getLog2PowerOf2(unsigned powerOf2);

/** Given a bitOffset return the bitIndex part of the bitOffset. */
unsigned getBitIndex(unsigned bitOffset);

/** Given a bitOffset return the byte offset part of the bitOffset */
unsigned getOffset(unsigned bitOffset);

/** Return bit at offset bitOffset in array[]; i.e., return
 *  (bits(array))[bitOffset]
 */
int getBitAtOffset(const Byte array[], unsigned bitOffset);

/** Set bit selected by bitOffset in array to bit. */
void setBitAtOffset(Byte array[], unsigned bitOffset, unsigned bit);

/** Set count bits in array[] starting at bitOffset to bit.  Return
 *  bit-offset one beyond last bit set.
 */
unsigned setBitsAtOffset(Byte array[], unsigned bitOffset,
                         unsigned bit, unsigned count);


/** Return count of run of identical bits starting at bitOffset
 *  in bytes[nBytes].
 */
unsigned runLength(const Byte bytes[], unsigned nBytes, unsigned bitOffset);

#endif //ifdef DO_TESTS

#endif //ifndef TEST_H_
