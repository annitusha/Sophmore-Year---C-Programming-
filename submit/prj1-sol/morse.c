#include "morse.h"
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>

typedef struct {
  char c;
  const char *code;
} TextMorse;

//<https://en.wikipedia.org/wiki/Morse_code#/media/File:International_Morse_Code.svg>
static const TextMorse charCodes[] = {
  { 'A', ".-" },
  { 'B', "-..." },
  { 'C', "_._." },
  { 'D', "-.." },
  { 'E', "." },
  { 'F', "..-." },
  { 'G', "--." },
  { 'H', "...." },
  { 'I', ".." },
  { 'J', ".---" },
  { 'K', "-.-" },
  { 'L', ".-.." },
  { 'M', "--" },
  { 'N', "-." },
  { 'O', "---" },
  { 'P', ".--." },
  { 'Q', "--.-" },
  { 'R', ".-." },
  { 'S', "..." },
  { 'T', "-" },
  { 'U', "..-" },
  { 'V', "...-" },
  { 'W', ".--" },
  { 'X', "-..-" },
  { 'Y', "-.--" },
  { 'Z', "--.." },

  { '1', ".----" },
  { '2', "..---" },
  { '3', "...--" },
  { '4', "....-" },
  { '5', "....." },
  { '6', "-...." },
  { '7', "--..." },
  { '8', "---.." },
  { '9', "----." },
  { '0', "-----" },

  { '\0', ".-.-." }, //AR Prosign indicating End-of-message
                     //<https://en.wikipedia.org/wiki/Prosigns_for_Morse_code>
};


/** Return NUL-terminated code string for char c. Returns NULL if
 *  there is no code for c.
 */
static const char *
charToCode(Byte c) {
  for (int i = 0; i < sizeof(charCodes)/sizeof(charCodes[0]); i++) {
    if (charCodes[i].c == c) return charCodes[i].code;
  }
  return NULL;
}


/** Return char for code. Returns < 0 if code is invalid.
 */
static int
codeToChar(const char *code) {
  for (int i = 0; i < sizeof(charCodes)/sizeof(charCodes[0]); i++) {
    if (strcmp(charCodes[i].code, code) == 0) return charCodes[i].c;
  }
  return -1;
}

/** Given an array of Bytes, a bit index is the offset of a bit
 *  in the array (with MSB having offset 0).
 *
 *  Given a bytes[] array and some bitOffset, and assuming that
 *  BITS_PER_BYTE is 8, then (bitOffset >> 3) represents the index of
 *  the byte within bytes[] and (bitOffset & 0x7) gives the bit-index
 *  within the byte (MSB represented by bit-index 0) and .
 *
 *  For example, given array a[] = {0xB1, 0xC7} which is
 *  { 0b1011_0001, 0b1100_0111 } we have the following:
 *
 *     Bit-Offset   Value
 *        0           1
 *        1           0
 *        2           1
 *        3           1
 *        4           0
 *        5           0
 *        6           0
 *        7           1
 *        8           1
 *        9           1
 *       10           0
 *       11           0
 *       12           0
 *       13           1
 *       14           1
 *       15           1
 *
 */


/** Return mask for a Byte with bit at bitIndex set to 1, all other
 *  bits set to 0.  Note that bitIndex == 0 represents the MSB,
 *  bitIndex == 1 represents the next significant bit and so on.
 */
static inline unsigned byteBitMask(unsigned bitIndex)
{
    return 1 << (BITS_PER_BYTE - bitIndex - 1);
}

/** Given a power-of-2 powerOf2, return log2(powerOf2) */
static inline unsigned
getLog2PowerOf2(unsigned powerOf2)
{
  unsigned count = 0;
  unsigned value = 1;
  while(value != powerOf2) {
    count++;
    value <<=1;
  }
  return count;
}

/** Given a bitOffset return the bitIndex part of the bitOffset. */
static inline unsigned
getBitIndex(unsigned bitOffset)
{
  //TODO
  return bitOffset & getLog2PowerOf2(pow(2.0, (double) BITS_PER_BYTE - 1));
}

/** Given a bitOffset return the byte offset part of the bitOffset */
static inline unsigned
getOffset(unsigned bitOffset)
{
  //TODO
  return bitOffset >> (BITS_PER_BYTE == 8 ? 3 : 4);
}

/** Return bit at offset bitOffset in array[]; i.e., return
 *  (bits(array))[bitOffset]
 */
static inline int
getBitAtOffset(const Byte array[], unsigned bitOffset)
{
  //TODO
  unsigned bitMask = byteBitMask(getBitIndex(bitOffset));
  unsigned byteOffset = getOffset(bitOffset);
  return (array[byteOffset] & bitMask) != 0 ? 1 : 0;
}

/** Set bit selected by bitOffset in array to bit. */
static inline void
setBitAtOffset(Byte array[], unsigned bitOffset, unsigned bit)
{
  //TODO
  int bitIndex = getBitIndex(bitOffset);
  int byteIndex = getOffset(bitOffset);
  int b = array[byteIndex];
  b = b & ~byteBitMask(bitIndex);
  if(bit > 0)
      b = b | byteBitMask(bitIndex);
  array[byteIndex] = b;
  return;
}

/** Set count bits in array[] starting at bitOffset to bit.  Return
 *  bit-offset one beyond last bit set.
 */
static inline unsigned
setBitsAtOffset(Byte array[], unsigned bitOffset, unsigned bit, unsigned count)
{
  //TODO
  for(int i = bitOffset; i< bitOffset + count; i++)
      setBitAtOffset(array, i, bit);
  return bitOffset + count;
}


/** Convert text[nText] into a binary encoding of morse code in
 *  morse[].  It is assumed that array morse[] is initially all zero
 *  and is large enough to represent the morse code for all characters
 *  in text[].  The result in morse[] should be terminated by the
 *  morse prosign AR.  Any sequence of non-alphanumeric characters in
 *  text[] should be treated as a *single* inter-word space.  Leading
 *  non alphanumeric characters in text are ignored.
 *
 *  Returns count of number of bytes used within morse[].
 */
int
textToMorse(const Byte text[], unsigned nText, Byte morse[])
{
        unsigned bitOffset = 0;

        for (int i = 0; i <= nText; i++) {
            Byte c = i < nText ? text[i] : '\0';
            if(bitOffset == 0 && !isalnum(c)) continue;
            const char * morseCode = charToCode(c);
            if(morseCode != NULL) {
                int codeIndex = 0;
                while (morseCode[codeIndex] != '\0') {
                    const char sym = morseCode[codeIndex++];  //'.' or '-'
                    if(sym == '.') {
                        setBitAtOffset(morse, bitOffset, 1);
                        bitOffset++;
                    } else {
                        setBitsAtOffset(morse, bitOffset, 1, 3);
                        bitOffset += 3;
                    }
                    setBitAtOffset(morse, bitOffset, 0);
                    bitOffset++;
                }
                setBitsAtOffset(morse, bitOffset, 0, 2);
                bitOffset += 2;
            } else {
                setBitsAtOffset(morse, bitOffset, 0, 4);
                bitOffset += 4;
            }
        }
        //TODO
        return getOffset(bitOffset) + 1;
}

/** Return count of run of identical bits starting at bitOffset
 *  in bytes[nBytes].
 */
static inline unsigned
runLength(const Byte bytes[], unsigned nBytes, unsigned bitOffset)
{
  //TODO
    int bitCount = 0;
    int totalBits = nBytes * BITS_PER_BYTE;
    if(bitOffset < totalBits) {
        int startBit = getBitAtOffset(bytes, bitOffset);
        while (bitOffset < totalBits && getBitAtOffset(bytes, bitOffset) == startBit) {
            bitOffset++;
            bitCount++;
        }
    }


    return bitCount;
}


/** Convert AR-prosign terminated binary Morse encoding in
 *  morse[nMorse] into text in text[].  It is assumed that array
 *  text[] is large enough to represent the decoding of the code in
 *  morse[].  Leading zero bits in morse[] are ignored. Encodings
 *  representing word separators are output as a space ' ' character.
 *
 *  Returns count of number of bytes used within text[], < 0 on error.
 */
int
morseToText(const Byte morse[], unsigned nMorse, Byte text[])
{
        //TODO
        char code[6];
        int allBitsCount = nMorse * BITS_PER_BYTE;
        int bitOffset = 0;
        int charIndex = 0;
        int textIndex = 0;
        while(bitOffset < allBitsCount) {
        int currentBit = getBitAtOffset(morse, bitOffset);
        int bitCount = runLength(morse, nMorse, bitOffset);
        if(currentBit == 1) {
            if(bitCount != 1 && bitCount != 3)
                return -1;
            code[charIndex] = (bitCount == 1) ? '.' : '-';
            charIndex++;
        }
        else {
            if(bitCount >= 3) {
                code[charIndex] = '\0';
                char ch = codeToChar(code);
                if(bitCount > 7 && ch != '\0') {
                    return -1;
                }
                text[textIndex] = ch;
                textIndex++;
                if(bitCount == 7) {
                    text[textIndex] = ' ';
                    textIndex++;
                }
                memset(code, 0, sizeof code);
                charIndex = 0;
            }

        }
        bitOffset += bitCount;
    }
    return textIndex - 1;
}
