#ifndef morse_h_
#define morse_h_

#include <limits.h>  //for CHAR_BIT

#ifndef BYTE_SIZE

typedef unsigned char Byte;
#define BYTE_SIZE 1

#endif

#include "tests.h"

//assume a power-of-2
enum { BITS_PER_BYTE = CHAR_BIT*sizeof(Byte) };

/**
Morse code binary encoding
<https://en.wikipedia.org/wiki/Morse_code#Transmission>.

A 1 represents sound, a 0 represents silence.

  Dot symbol represented by a single 1
  Dash symbol represented as 3 1's.
  Separation between dots and dash symbols: a single 0
  Separation between letters: 3 0's
  Separation between words: 7 0's


The end of a message is indicated by the AR prosign; i.e. the morse
code .- for A followed by the code for R .-. without any inter-letter
separation followed by at least 3 0's.

0's collapse to the minimum necessary; i.e. we will never have more
than 7 successive 0's.

For example, the morse code for "S" is "..." and that for "O" is "---".
Hence the code for "SOS" is "... --- ..." which would be encoded as
10101000 11101110111000 10101000; grouping into bytes:

1010_1000 1110_1110 1110_0010 1010_00--
 . .  .    -    -    -      .  . .
       S                 O         S

Adding the AR end-of-message prosign (no extra 0's between the A and
the R) which is .-.-.

1011101011101000
 .   - .   - .
     A        R

we would get:

1010_1000 1110_1110 1110_0010 1010_0010 1110_1011 1010_0000
 . .  .      -    -    -    .  . .    .    -  .    - .
       S                 O         S       A           R

Note the two extra 0's at the end due to padding out to a byte
boundary.
 */


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
int textToMorse(const Byte text[], unsigned nText, Byte morse[]);


/** Convert AR-prosign terminated binary Morse encoding in
 *  morse[nMorse] into text in text[].  It is assumed that array
 *  text[] is large enough to represent the decoding of the code in
 *  morse[].  Leading zero bits in morse[] are ignored.  Encodings
 *  representing word separators are output as a space ' ' character.
 *
 *  Returns count of number of bytes used within text[], < 0 on error.
 */
int morseToText(const Byte morse[], unsigned nMorse, Byte text[]);


#endif //ifndef morse_h_
