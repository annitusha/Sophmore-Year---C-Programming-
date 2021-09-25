#include "morse.h"

#include <check.h>

#include <assert.h>

/************************** byteBitMask() Tests ************************/

START_TEST(byteBitMask_Msb0)
{
  unsigned result = byteBitMask(0);

  ck_assert_int_eq(result, 1 << (BITS_PER_BYTE - 1));
}
END_TEST

START_TEST(byteBitMask_Msb1)
{
  unsigned result = byteBitMask(1);

  ck_assert_int_eq(result, 1 << (BITS_PER_BYTE - 2));
}
END_TEST

START_TEST(byteBitMask_Lsb1)
{

  unsigned result = byteBitMask(BITS_PER_BYTE - 2);

  ck_assert_int_eq(result, 2);
}
END_TEST


START_TEST(byteBitMask_Lsb0)
{

  unsigned result = byteBitMask(BITS_PER_BYTE - 1);

  ck_assert_int_eq(result, 1);
}
END_TEST

static void
add_byteBitMask_tests(Suite *suite)
{
  TCase *byteBitMaskTests = tcase_create("byteBitMask");
  tcase_add_test(byteBitMaskTests, byteBitMask_Msb0);
  tcase_add_test(byteBitMaskTests, byteBitMask_Msb1);
  tcase_add_test(byteBitMaskTests, byteBitMask_Lsb1);
  tcase_add_test(byteBitMaskTests, byteBitMask_Lsb0);
  suite_add_tcase(suite, byteBitMaskTests);
}

/************************ getLog2PowerOf2() Tests **********************/

START_TEST(getLog2PowerOf2_1)
{

  unsigned result = getLog2PowerOf2(1);

  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(getLog2PowerOf2_8)
{

  unsigned result = getLog2PowerOf2(8);

  ck_assert_int_eq(result, 3);
}
END_TEST

START_TEST(getLog2PowerOf2_16)
{

  unsigned result = getLog2PowerOf2(16);

  ck_assert_int_eq(result, 4);
}
END_TEST

START_TEST(getLog2PowerOf2_32)
{

  unsigned result = getLog2PowerOf2(32);

  ck_assert_int_eq(result, 5);
}
END_TEST

START_TEST(getLog2PowerOf2_512)
{

  unsigned result = getLog2PowerOf2(512);

  ck_assert_int_eq(result, 9);
}
END_TEST

static void
add_getLog2PowerOf2_tests(Suite *suite)
{
  TCase *getLog2PowerOf2Tests = tcase_create("getLog2PowerOf2");
  tcase_add_test(getLog2PowerOf2Tests, getLog2PowerOf2_1);
  tcase_add_test(getLog2PowerOf2Tests, getLog2PowerOf2_8);
  tcase_add_test(getLog2PowerOf2Tests, getLog2PowerOf2_16);
  tcase_add_test(getLog2PowerOf2Tests, getLog2PowerOf2_32);
  tcase_add_test(getLog2PowerOf2Tests, getLog2PowerOf2_512);
  suite_add_tcase(suite, getLog2PowerOf2Tests);
}

/************************** getBitIndex() Tests ************************/

START_TEST(getBitIndex_0x1a3f)
{

  unsigned result = getBitIndex(0x1a3f);

  ck_assert_int_eq(result, BYTE_SIZE == 2 ? 0xf : 0x7);
}
END_TEST

START_TEST(getBitIndex_0x1a39)
{

  unsigned result = getBitIndex(0x1a39);

  ck_assert_int_eq(result, BYTE_SIZE == 2 ? 0x9 : 0x1);
}
END_TEST

START_TEST(getBitIndex_0x1a3)
{

  unsigned result = getBitIndex(0x1a3);

  ck_assert_int_eq(result, 3);
}
END_TEST

static void
add_getBitIndex_tests(Suite *suite)
{
  TCase *getBitIndexTests = tcase_create("getBitIndex");
  tcase_add_test(getBitIndexTests, getBitIndex_0x1a3f);
  tcase_add_test(getBitIndexTests, getBitIndex_0x1a39);
  tcase_add_test(getBitIndexTests, getBitIndex_0x1a3);
  suite_add_tcase(suite, getBitIndexTests);
}

/************************** getOffset() Tests **************************/

START_TEST(getOffset_0x1a3f)
{

  unsigned result = getOffset(0x1a3f);

  ck_assert_int_eq(result, BYTE_SIZE == 2 ? 0x1a3 : 0x347);
}
END_TEST

START_TEST(getOffset_0x1a39)
{

  unsigned result = getOffset(0x1a39);

  ck_assert_int_eq(result, BYTE_SIZE == 2 ? 0x1a3 : 0x347);
}
END_TEST

START_TEST(getOffset_0x1a3)
{

  unsigned result = getOffset(0x1a3);

  ck_assert_int_eq(result, BYTE_SIZE == 2 ? 0x1a : 0x34);
}
END_TEST

static void
add_getOffset_tests(Suite *suite)
{
  TCase *getOffsetTests = tcase_create("getOffset");
  tcase_add_test(getOffsetTests, getOffset_0x1a3f);
  tcase_add_test(getOffsetTests, getOffset_0x1a39);
  tcase_add_test(getOffsetTests, getOffset_0x1a3);
  suite_add_tcase(suite, getOffsetTests);
}

/************************ getBitAtOffset() Tests ***********************/

START_TEST(getBitAtOffset_15)
{

  Byte bytes[] = { 0x1a, 0x23, 0x44 };

  unsigned result = getBitAtOffset(bytes, BYTE_SIZE == 2 ? 31 : 15);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(getBitAtOffset_13)
{

  Byte bytes[] = { 0x1a, 0x23, 0x44 };

  unsigned result = getBitAtOffset(bytes, BYTE_SIZE == 2 ? 29 : 13);

  ck_assert_int_eq(result, 0);
}
END_TEST

static void
add_getBitAtOffset_tests(Suite *suite)
{
  TCase *getBitAtOffsetTests = tcase_create("getBitAtOffset");
  tcase_add_test(getBitAtOffsetTests, getBitAtOffset_15);
  tcase_add_test(getBitAtOffsetTests, getBitAtOffset_13);
  suite_add_tcase(suite, getBitAtOffsetTests);
}

/************************ setBitAtOffset() Tests ***********************/

START_TEST(setBitAtOffset_15_0)
{

  Byte bytes[] = { 0x1a, 0x23, 0x44 };

  setBitAtOffset(bytes, BYTE_SIZE == 2 ? 31 : 15, 0);

  ck_assert_int_eq(bytes[0], 0x1a);
  ck_assert_int_eq(bytes[1], 0x22);
  ck_assert_int_eq(bytes[2], 0x44);
}
END_TEST

START_TEST(setBitAtOffset_15_1)
{

  Byte bytes[] = { 0x1a, 0x23, 0x44 };

  setBitAtOffset(bytes, BYTE_SIZE == 2 ? 31 : 15, 1);

  ck_assert_int_eq(bytes[0], 0x1a);
  ck_assert_int_eq(bytes[1], 0x23);
  ck_assert_int_eq(bytes[2], 0x44);
}
END_TEST

START_TEST(setBitAtOffset_13_0)
{

  Byte bytes[] = { 0x1a, 0x23, 0x44 };

  setBitAtOffset(bytes, BYTE_SIZE == 2 ? 29 : 13, 0);

  ck_assert_int_eq(bytes[0], 0x1a);
  ck_assert_int_eq(bytes[1], 0x23);
  ck_assert_int_eq(bytes[2], 0x44);
}
END_TEST

START_TEST(setBitAtOffset_13_1)
{

  Byte bytes[] = { 0x1a, 0x23, 0x44 };

  setBitAtOffset(bytes, BYTE_SIZE == 2 ? 29 : 13, 1);

  ck_assert_int_eq(bytes[0], 0x1a);
  ck_assert_int_eq(bytes[1], 0x27);
  ck_assert_int_eq(bytes[2], 0x44);
}
END_TEST

START_TEST(setBitAtOffset_16_0)
{

  Byte bytes[] = { 0x1a, 0x23, (BYTE_SIZE == 2) ? 0x8004 : 0x84 };

  setBitAtOffset(bytes, BYTE_SIZE == 2 ? 32 : 16, 0);

  ck_assert_int_eq(bytes[0], 0x1a);
  ck_assert_int_eq(bytes[1], 0x23);
  ck_assert_int_eq(bytes[2], 0x04);
}
END_TEST

START_TEST(setBitAtOffset_16_1)
{

  Byte bytes[] = { 0x1a, 0x23, 0x04  };

  setBitAtOffset(bytes, BYTE_SIZE == 2 ? 32 : 16, 1);

  ck_assert_int_eq(bytes[0], 0x1a);
  ck_assert_int_eq(bytes[1], 0x23);
  ck_assert_int_eq(bytes[2], (BYTE_SIZE == 2) ? 0x8004 : 0x84);
}
END_TEST

static void
add_setBitAtOffset_tests(Suite *suite)
{
  TCase *setBitAtOffsetTests = tcase_create("setBitAtOffset");
  tcase_add_test(setBitAtOffsetTests, setBitAtOffset_15_0);
  tcase_add_test(setBitAtOffsetTests, setBitAtOffset_15_1);
  tcase_add_test(setBitAtOffsetTests, setBitAtOffset_13_0);
  tcase_add_test(setBitAtOffsetTests, setBitAtOffset_13_1);
  tcase_add_test(setBitAtOffsetTests, setBitAtOffset_16_0);
  tcase_add_test(setBitAtOffsetTests, setBitAtOffset_16_1);
  suite_add_tcase(suite, setBitAtOffsetTests);
}

/************************ setBitsAtOffset() Tests **********************/

START_TEST(setBitsAtOffset_12_0_4)
{

  Byte bytes[] = { 0x1a, 0x23, 0x44 };

  setBitsAtOffset(bytes, BYTE_SIZE == 2 ? 28 : 12, 0, 4);

  ck_assert_int_eq(bytes[0], 0x1a);
  ck_assert_int_eq(bytes[1], 0x20);
  ck_assert_int_eq(bytes[2], 0x44);
}
END_TEST

START_TEST(setBitsAtOffset_12_1_4)
{

  Byte bytes[] = { 0x1a, 0x23, 0x44 };

  setBitsAtOffset(bytes, BYTE_SIZE == 2 ? 28 : 12, 1, 4);

  ck_assert_int_eq(bytes[0], 0x1a);
  ck_assert_int_eq(bytes[1], 0x2f);
  ck_assert_int_eq(bytes[2], 0x44);
}
END_TEST

START_TEST(setBitsAtOffset_12_0_5)
{

  Byte bytes[] = { 0x1a, 0x23, 0x44 };

  setBitsAtOffset(bytes, BYTE_SIZE == 2 ? 28 : 12, 0, 5);

  ck_assert_int_eq(bytes[0], 0x1a);
  ck_assert_int_eq(bytes[1], 0x20);
  ck_assert_int_eq(bytes[2], 0x44);
}
END_TEST

START_TEST(setBitsAtOffset_12_1_5)
{

  Byte bytes[] = { 0x1a, 0x23, 0x44 };

  setBitsAtOffset(bytes, BYTE_SIZE == 2 ? 28 : 12, 1, 5);

  ck_assert_int_eq(bytes[0], 0x1a);
  ck_assert_int_eq(bytes[1], 0x2f);
  ck_assert_int_eq(bytes[2], (BYTE_SIZE == 2) ? 0x8044 : 0xc4);
}
END_TEST



static void
add_setBitsAtOffset_tests(Suite *suite)
{
  TCase *setBitsAtOffsetTests = tcase_create("setBitsAtOffset");
  tcase_add_test(setBitsAtOffsetTests, setBitsAtOffset_12_0_4);
  tcase_add_test(setBitsAtOffsetTests, setBitsAtOffset_12_1_4);
  tcase_add_test(setBitsAtOffsetTests, setBitsAtOffset_12_0_5);
  tcase_add_test(setBitsAtOffsetTests, setBitsAtOffset_12_1_5);
  suite_add_tcase(suite, setBitsAtOffsetTests);
}

/*************************** runLength() Tests *************************/

START_TEST(runLength_12_1)
{

  Byte bytes[] = { 0x1a, 0x2e, 0x44 };
  const int nBytes = sizeof(bytes)/sizeof(bytes[0]);

  unsigned result = runLength(bytes, nBytes, BYTE_SIZE == 2 ? 28 : 12);

  ck_assert_int_eq(result, 3);
}
END_TEST

START_TEST(runLength_12_1_boundary)
{

  Byte bytes[] = { 0x1a, 0x2f, (BYTE_SIZE == 2) ? 0xc044 : 0xc4};
  const int nBytes = sizeof(bytes)/sizeof(bytes[0]);

  unsigned result = runLength(bytes, nBytes, BYTE_SIZE == 2 ? 28 : 12);

  ck_assert_int_eq(result, 6);
}
END_TEST

START_TEST(runLength_28_0_end)
{

  Byte bytes[] = { 0x1a, 0x2e, 0x44 };
  const int nBytes = sizeof(bytes)/sizeof(bytes[0]);

  unsigned result = runLength(bytes, nBytes, BYTE_SIZE == 2 ? 46 : 22);

  ck_assert_int_eq(result, 2);
}
END_TEST

static void
add_runLength_tests(Suite *suite)
{
  TCase *runLengthTests = tcase_create("runLength");
  tcase_add_test(runLengthTests, runLength_12_1);
  tcase_add_test(runLengthTests, runLength_28_0_end);
  tcase_add_test(runLengthTests, runLength_12_1_boundary);
  suite_add_tcase(suite, runLengthTests);
}

/********************* Morse Encode / Decode Tests *********************/

const Byte SOS[] = { 'S', 'O', 'S' };
const unsigned nSOS = sizeof(SOS)/sizeof(SOS[0]);

/*
1 0 1 0 1 0 0 0   1 1 1 0 1 1 1 0   1 1 1 0 0 0 1 0    1 0 1 0 0 0 1 0
  .   .   .             -       -         -       .      .   .       .
              S                               O                  S
0xa8              0xee              0xe2               0xa2


1 1 1 0 1 0 1 1   1 0 1 0 0 0 0 0
      -   .         -   .
        A                 R
0xeb              0xa0
 */

#if BYTE_SIZE == 2
static Byte sosBin[] = { 0xa8ee, 0xe2a2, 0xeba0, };
#else
static Byte sosBin[] = { 0xa8, 0xee, 0xe2, 0xa2, 0xeb, 0xa0, };
#endif

static unsigned nSosBin = sizeof(sosBin)/sizeof(sosBin[0]);


START_TEST(textToMorse_sos)
{

  Byte bytes[] = { [9] = 0 }; //force all 0's

  const int result = textToMorse(SOS, nSOS, bytes);
  ck_assert_int_eq(result, nSosBin);
  for (int i = 0; i < nSosBin; i++) {
    ck_assert_int_eq(bytes[i], sosBin[i]);
  }

}
END_TEST

START_TEST(morseToText_sos)
{

  Byte text[] = { [9] = 0 }; //force all 0's

  const int result = morseToText(sosBin, nSosBin, text);
  ck_assert_int_eq(result, nSOS);
  for (int i = 0; i < nSOS; i++) {
    ck_assert_int_eq(text[i], SOS[i]);
  }

}
END_TEST

static void
add_encodeDecode_tests(Suite *suite)
{
  TCase *encodeDecodeTests = tcase_create("encodeDecode");
  tcase_add_test(encodeDecodeTests, textToMorse_sos);
  tcase_add_test(encodeDecodeTests, morseToText_sos);
  suite_add_tcase(suite, encodeDecodeTests);
}


/*************************** Main Test Function ************************/

int
main(void)
{
  assert(sizeof(unsigned short) == 2);

  Suite *suite = suite_create("morse-tests");

  add_byteBitMask_tests(suite);
  add_getLog2PowerOf2_tests(suite);
  add_getBitIndex_tests(suite);
  add_getOffset_tests(suite);
  add_getBitAtOffset_tests(suite);
  add_setBitAtOffset_tests(suite);
  add_setBitsAtOffset_tests(suite);
  add_runLength_tests(suite);
  add_encodeDecode_tests(suite);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int nFail = srunner_ntests_failed(runner);
  srunner_free(runner);

  return nFail != 0;
}
