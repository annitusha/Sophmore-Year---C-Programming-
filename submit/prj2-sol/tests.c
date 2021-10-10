#include "int-set.h"
#include "int-set-strings.h"

#include <check.h>

#include <assert.h>
#include <stdlib.h>

/*************************** newIntSet() Tests *************************/

START_TEST(newIntSetAndFree)
{
  void *set = newIntSet();
  unsigned result = nElementsIntSet(set);
  freeIntSet(set);
  ck_assert_int_eq(result, 0);
}
END_TEST

static Suite *
newIntSetSuite(void)
{
  Suite *suite = suite_create("newIntSet");
  TCase *newIntSetTests = tcase_create("newIntSet");
  tcase_add_test(newIntSetTests, newIntSetAndFree);
  suite_add_tcase(suite, newIntSetTests);
  return suite;
}

/*************************** addIntSet Tests ***************************/

START_TEST(addElement)
{
  void *set = newIntSet();
  int result1 = addIntSet(set, 33);
  int result2 = addIntSet(set, -22);
  int result3 = addIntSet(set, 33);  //duplicate should be ignored
  ck_assert_int_eq(result1, 1);
  ck_assert_int_eq(result2, 2);
  ck_assert_int_eq(result3, 2);
  ck_assert_int_eq(nElementsIntSet(set), 2);
  freeIntSet(set);
}
END_TEST

static Suite *
addIntSetSuite(void)
{
  Suite *suite = suite_create("addIntSet");
  TCase *addElementTests = tcase_create("addElement");
  tcase_add_test(addElementTests, addElement);
  suite_add_tcase(suite, addElementTests);
  return suite;
}


/*********************** nElementsIntSet Tests ************************/


START_TEST(nElements)
{
  void *set = newIntSet();
  int result1 = addIntSet(set, -33);
  int result2 = addIntSet(set, 22);
  int result3 = addIntSet(set, 66);
  int n = nElementsIntSet(set);
  freeIntSet(set);
  ck_assert_int_eq(result1, 1);
  ck_assert_int_eq(result2, 2);
  ck_assert_int_eq(result3, 3);
  ck_assert_int_eq(n, 3);
}
END_TEST


static Suite *
nElementsIntSetSuite(void)
{
  Suite *suite = suite_create("nElementsIntSet");
  TCase *tests = tcase_create("sizeBitSet");
  tcase_add_test(tests, nElements);
  suite_add_tcase(suite, tests);
  return suite;
}


/********************** addMultipleIntSet Tests ************************/

START_TEST(multiAdd)
{
  void *set = newIntSet();
  const int elements[] = { 33, -53, 54, 2, 0, -1 };
  const int nElements = sizeof(elements)/sizeof(elements[0]);
  int n = addMultipleIntSet(set, elements, nElements);
  freeIntSet(set);
  ck_assert_int_eq(n, nElements);
}
END_TEST


static Suite *
addMultipleIntSetSuite(void)
{
  Suite *suite = suite_create("addMultipleIntSet");
  TCase *tests = tcase_create("addMultiple");
  tcase_add_test(tests, multiAdd);
  suite_add_tcase(suite, tests);
  return suite;
}


/************************* isInIntSet Tests ****************************/

START_TEST(contains)
{
  void *set = newIntSet();
  const int elements[] = { 33, 53, 33, 54, 2, };
  const int nElements = sizeof(elements)/sizeof(elements[0]);
  int n = addMultipleIntSet(set, elements, nElements);
  ck_assert_int_eq(n, nElements - 1);
  for (int i = 0; i < nElements; i++) {
    ck_assert_int_eq(isInIntSet(set, elements[i]), 1);
  }
  freeIntSet(set);

}
END_TEST


START_TEST(notContains)
{
  void *set = newIntSet();
  const int elements[] = { 33, 53, 33, 54, 2, };
  const int nElements = sizeof(elements)/sizeof(elements[0]);
  int n = addMultipleIntSet(set, elements, nElements);
  ck_assert_int_eq(n, nElements - 1);
  ck_assert_int_eq(isInIntSet(set, 44), 0);
  ck_assert_int_eq(isInIntSet(set, 55), 0);
  freeIntSet(set);

}
END_TEST

static Suite *
isInIntSetSuite(void)
{
  Suite *suite = suite_create("isInIntSet()");
  TCase *tests = tcase_create("isIn");
  tcase_add_test(tests, contains);
  tcase_add_test(tests, notContains);
  suite_add_tcase(suite, tests);
  return suite;
}


/**************************** iterator Tests ***************************/

START_TEST(iterator)
{
  void *set = newIntSet();
  const int elements[] = { 33, 53, 54, -22, 33, -22 };
  const int nElements = sizeof(elements)/sizeof(elements[0]);
  const int sortElements[] = { -22, 33, 53, 54 };
  const int nSortElements = sizeof(sortElements)/sizeof(sortElements[0]);
  int n = addMultipleIntSet(set, elements, nElements);
  ck_assert_int_eq(n, nSortElements);
  int i = 0;
  for (const void *iter = newIntSetIterator(set); iter != NULL;
       iter = stepIntSetIterator(iter)) {
    int v = intSetIteratorElement(iter);
    ck_assert_int_eq(v, sortElements[i++]);
  }
  ck_assert_int_eq(i, nSortElements);
  freeIntSet(set);
}
END_TEST

START_TEST(manyElementsIter)
{
  void *set = newIntSet();
  const int LO = -1000; //inclusive
  const int HI = 1000;  //exclusive
  const int K = 5;
  for (int k = 0; k < 5; k++) {
    for (int i = LO; i < HI; i += K) addIntSet(set, i + k);
  }
  ck_assert_int_eq(nElementsIntSet(set), HI - LO);
  int i = LO;
  for (const void *iter = newIntSetIterator(set); iter != NULL;
       iter = stepIntSetIterator(iter)) {
    int v = intSetIteratorElement(iter);
    ck_assert_int_eq(v, i++);
  }
  ck_assert_int_eq(i, HI);
  freeIntSet(set);
}
END_TEST


static Suite *
iteratorSuite(void)
{
  Suite *suite = suite_create("iterator");
  TCase *tests = tcase_create("iterator");
  tcase_add_test(tests, iterator);
  tcase_add_test(tests, manyElementsIter);
  suite_add_tcase(suite, tests);
  return suite;
}


/****************************** scan Tests *****************************/

static void
scanTest(const char *str, int arr[], int nArr, int expectErr)
{
  int n;
  void *set = sscanIntSet(str, &n);
  if (expectErr) {
    ck_assert_ptr_eq(set, NULL);  //ck_assert_ptr_null() not declared!!
  }
  else {
    ck_assert_ptr_ne(set, NULL); //ck_assert_ptr_nonnumm() not declared!!
    ck_assert_int_eq(n, strlen(str));
    int i = 0;
    for (const void *iter = newIntSetIterator(set); iter != NULL;
         iter = stepIntSetIterator(iter)) {
      int v = intSetIteratorElement(iter);
      ck_assert_int_eq(v, arr[i++]);
    }
    ck_assert_int_eq(i, nArr);
  }
  if (set != NULL) freeIntSet(set);
}

START_TEST(scanEmpty)
{
  scanTest("{}", NULL, 0, 0);
}
END_TEST

START_TEST(scanEmptyWhitespace)
{
  scanTest("  { \t \n }", NULL, 0, 0);
}
END_TEST

START_TEST(scan1Element)
{
  scanTest("  { 22 }", (int[1]) { 22 }, 1, 0);
}
END_TEST

START_TEST(scan1Negative)
{
  scanTest("  { -22 }", (int[1]) { -22 }, 1, 0);
}
END_TEST

START_TEST(scan1ElementRepeat)
{
  scanTest("  { 22, 22 }", (int[1]) { 22 }, 1, 0);
}
END_TEST

START_TEST(scan1ElementTrailingComma)
{
  scanTest("  { 22 ,}", (int[1]) { 22 }, 1, 0);
}
END_TEST
START_TEST(scan3Elements)
{
  scanTest("  { 44, 22, 33 }", (int[3]) { 22, 33, 44 }, 3, 0);
}
END_TEST

START_TEST(scan3ElementsRepeat)
{
  scanTest("  { 44, 22, 33, 44, 22, 33 }", (int[3]) { 22, 33, 44 }, 3, 0);
}
END_TEST

START_TEST(scanMulti)
{
  scanTest("  { -44, 22, -33, 44, 22, 33 }",
           (int[5]) { -44, -33, 22, 33, 44 }, 5, 0);
}
END_TEST

START_TEST(scanNoLBraceErr)
{
  scanTest("  44, 22, 33, 44, 22, 33 }", NULL, 0, 1);
}
END_TEST

START_TEST(scanNoRBraceErr)
{
  scanTest(" { 44, 22, 33, 44, 22, 33 ", NULL, 0, 1);
}
END_TEST

START_TEST(scanExtraCommaErr)
{
  scanTest(" { 44, 22, 33, , 44, 22, 33 }", NULL, 0, 1);
}
END_TEST


static Suite *
sscanIntSetSuite(void)
{
  Suite *suite = suite_create("sscanIntSet");
  TCase *scanTests = tcase_create("iterator");
  tcase_add_test(scanTests, scanEmpty);
  tcase_add_test(scanTests, scanEmptyWhitespace);
  tcase_add_test(scanTests, scan1Element);
  tcase_add_test(scanTests, scan1Negative);
  tcase_add_test(scanTests, scan1ElementTrailingComma);
  tcase_add_test(scanTests, scan1ElementRepeat);
  tcase_add_test(scanTests, scan3Elements);
  tcase_add_test(scanTests, scan3ElementsRepeat);
  tcase_add_test(scanTests, scanMulti);
  tcase_add_test(scanTests, scanNoLBraceErr);
  tcase_add_test(scanTests, scanNoRBraceErr);
  tcase_add_test(scanTests, scanExtraCommaErr);
  suite_add_tcase(suite, scanTests);
  return suite;
}

/**************************** snprint Tests ****************************/

static void
snprintTest(const int arr[], int nArr, const char *str)
{
  void *set = newIntSet();
  for (int i = 0; i < nArr; i++) { addIntSet(set, arr[i]); }
  int n = snprintIntSet(set, NULL, 0);
  char buf[n + 1];
  int n1 = snprintIntSet(set, buf, n + 1);
  ck_assert_int_eq(n, n1);
  ck_assert_str_eq(buf, str);
  freeIntSet(set);
}

START_TEST(snprintEmpty)
{
  snprintTest(NULL, 0, "{ }");
}
END_TEST

START_TEST(snprint1)
{
  snprintTest((int[1]){ 22 }, 1, "{ 22, }");
}
END_TEST

START_TEST(snprint1Negative)
{
  snprintTest((int[1]){ -22 }, 1, "{ -22, }");
}
END_TEST

START_TEST(snprintMulti)
{
  snprintTest((int[6]){ -22, 33, -22, 1, 0, 33 }, 6,
              "{ -22, 0, 1, 33, }");
}
END_TEST

static Suite *
snprintIntSetSuite(void)
{
  Suite *suite = suite_create("snprintIntSet");
  TCase *snprintTests = tcase_create("snprint");
  tcase_add_test(snprintTests, snprintEmpty);
  tcase_add_test(snprintTests, snprint1);
  tcase_add_test(snprintTests, snprint1Negative);
  tcase_add_test(snprintTests, snprintMulti);
  suite_add_tcase(suite, snprintTests);
  return suite;
}

/************************** unionIntSet Tests **************************/

static void
unionTest(const int arr1[], int nArr1, const int arr2[], int nArr2,
          const int unionArr[], int nUnionArr)
{
  void *set1 = newIntSet();
  addMultipleIntSet(set1, arr1, nArr1);
  void *set2 = newIntSet();
  addMultipleIntSet(set2, arr2, nArr2);
  int n = unionIntSet(set1, set2);
  ck_assert_int_eq(n, nUnionArr);
  int i = 0;
  for (const void *iter = newIntSetIterator(set1); iter != NULL;
       iter = stepIntSetIterator(iter)) {
    int v = intSetIteratorElement(iter);
    ck_assert_int_eq(v, unionArr[i++]);
  }
  ck_assert_int_eq(i, nUnionArr);
  freeIntSet(set1);
  freeIntSet(set2);
}

START_TEST(emptyEmptyUnion)
{
  unionTest(NULL, 0, NULL, 0, NULL, 0);
}
END_TEST

START_TEST(emptyNonEmptyUnion)
{
  const int elements2[] = { 33, 54, 53, 33, 53 };
  const int nElements2 = sizeof(elements2)/sizeof(elements2[0]);
  const int unionElements[] = { 33, 53, 54, };
  const int nUnionElements = sizeof(unionElements)/sizeof(unionElements[0]);
  unionTest(NULL, 0, elements2, nElements2, unionElements, nUnionElements);
}
END_TEST

START_TEST(nonEmptyEmptyUnion)
{
  const int elements1[] = { 1, 3, 2, 3, 1 };
  const int nElements1 = sizeof(elements1)/sizeof(elements1[0]);
  const int unionElements[] = { 1, 2, 3, };
  const int nUnionElements = sizeof(unionElements)/sizeof(unionElements[0]);
  unionTest(elements1, nElements1, NULL, 0, unionElements, nUnionElements);
}
END_TEST

START_TEST(smallerLargerUnion)
{
  const int elements1[] = { 1, 3, 2, 3, 1 };
  const int nElements1 = sizeof(elements1)/sizeof(elements1[0]);
  const int elements2[] = { 33, 54, 53, 33, 53 };
  const int nElements2 = sizeof(elements2)/sizeof(elements2[0]);
  const int unionElements[] = { 1, 2, 3, 33, 53, 54, };
  const int nUnionElements = sizeof(unionElements)/sizeof(unionElements[0]);
  unionTest(elements1, nElements1, elements2, nElements2,
            unionElements, nUnionElements);
}
END_TEST

START_TEST(largerSmallerUnion)
{
  const int elements1[] = { 33, 54, 53, 33, 53 };
  const int nElements1 = sizeof(elements1)/sizeof(elements1[0]);
  const int elements2[] = { 1, 3, 2, 3, 1 };
  const int nElements2 = sizeof(elements2)/sizeof(elements2[0]);
  const int unionElements[] = { 1, 2, 3, 33, 53, 54, };
  const int nUnionElements = sizeof(unionElements)/sizeof(unionElements[0]);
  unionTest(elements1, nElements1, elements2, nElements2,
            unionElements, nUnionElements);
}
END_TEST

START_TEST(interleavedUnion)
{
  const int elements1[] = { 1, 33, 54, 3, 45,   };
  const int nElements1 = sizeof(elements1)/sizeof(elements1[0]);
  const int elements2[] = { 1, 3, 33, 45, 2, 53, 53 };
  const int nElements2 = sizeof(elements2)/sizeof(elements2[0]);
  const int unionElements[] = { 1, 2, 3, 33, 45, 53, 54, };
  const int nUnionElements = sizeof(unionElements)/sizeof(unionElements[0]);
  unionTest(elements1, nElements1, elements2, nElements2,
            unionElements, nUnionElements);
}
END_TEST

static Suite *
unionIntSetSuite(void)
{
  Suite *suite = suite_create("unionIntSet");
  TCase *unionTests = tcase_create("union");
  tcase_add_test(unionTests, emptyEmptyUnion);
  tcase_add_test(unionTests, emptyNonEmptyUnion);
  tcase_add_test(unionTests, nonEmptyEmptyUnion);
  tcase_add_test(unionTests, smallerLargerUnion);
  tcase_add_test(unionTests, largerSmallerUnion);
  tcase_add_test(unionTests, interleavedUnion);
  suite_add_tcase(suite, unionTests);
  return suite;
}

/*********************** intersectionIntSet Tests **********************/

static void
intersectionTest(const int arr1[], int nArr1, const int arr2[], int nArr2,
                 const int intersectionArr[], int nIntersectionArr)
{
  void *set1 = newIntSet();
  addMultipleIntSet(set1, arr1, nArr1);
  void *set2 = newIntSet();
  addMultipleIntSet(set2, arr2, nArr2);
  int n = intersectionIntSet(set1, set2);
  ck_assert_int_eq(n, nIntersectionArr);
  int i = 0;
  for (const void *iter = newIntSetIterator(set1); iter != NULL;
       iter = stepIntSetIterator(iter)) {
    int v = intSetIteratorElement(iter);
    ck_assert_int_eq(v, intersectionArr[i++]);
  }
  ck_assert_int_eq(i, nIntersectionArr);
  freeIntSet(set1);
  freeIntSet(set2);
}

START_TEST(emptyEmptyIntersection)
{
  intersectionTest(NULL, 0, NULL, 0, NULL, 0);
}
END_TEST

START_TEST(emptyNonEmptyIntersection)
{
  const int elements2[] = { 33, 54, 53, 33, 53 };
  const int nElements2 = sizeof(elements2)/sizeof(elements2[0]);
  intersectionTest(NULL, 0, elements2, nElements2, NULL, 0);
}
END_TEST

START_TEST(nonEmptyEmptyIntersection)
{
  const int elements1[] = { 1, 3, 2, 3, 1 };
  const int nElements1 = sizeof(elements1)/sizeof(elements1[0]);
  intersectionTest(elements1, nElements1, NULL, 0, NULL, 0);
}
END_TEST

START_TEST(firstTestIntersection)
{
  const int elements1[] = { 1, 33, 2, 53, 54 };
  const int nElements1 = sizeof(elements1)/sizeof(elements1[0]);
  const int elements2[] = { 33, 54, 53, 33, 53 };
  const int nElements2 = sizeof(elements2)/sizeof(elements2[0]);
  const int interElements[] = { 33, 53, 54, };
  const int nInterElements = sizeof(interElements)/sizeof(interElements[0]);
  intersectionTest(elements1, nElements1, elements2, nElements2,interElements, nInterElements);
}
END_TEST

START_TEST(noIntersection)
{
  const int elements1[] = { 33, 54, 53, 33, 53 };
  const int nElements1 = sizeof(elements1)/sizeof(elements1[0]);
  const int elements2[] = { 1, 3, 2, 3, 1 };
  const int nElements2 = sizeof(elements2)/sizeof(elements2[0]);
  intersectionTest(elements1, nElements1, elements2, nElements2, NULL, 0);
}
END_TEST

START_TEST(interLeavedIntersection)
{
  const int elements1[] = { 1, 33, 54, 3, 45,   };
  const int nElements1 = sizeof(elements1)/sizeof(elements1[0]);
  const int elements2[] = { 1, 3, 33, 45, 2, 54, 53 };
  const int nElements2 = sizeof(elements2)/sizeof(elements2[0]);
  const int interElements[] = { 1, 3, 33, 45, 54, };
  const int nInterElements = sizeof(interElements)/sizeof(interElements[0]);
  intersectionTest(elements1, nElements1, elements2, nElements2, interElements, nInterElements);
}
END_TEST

//TODO: add more intersection tests.


static Suite *
intersectionIntSetSuite(void)
{
  Suite *suite = suite_create("intersectionIntSet");
  TCase *intersectionTests = tcase_create("intersection");
  tcase_add_test(intersectionTests, emptyEmptyIntersection);
  tcase_add_test(intersectionTests, emptyNonEmptyIntersection);
  //TODO: for each test added above tcase_add_test(intersectionTests, ...)
  tcase_add_test(intersectionTests, nonEmptyEmptyIntersection);
  tcase_add_test(intersectionTests, firstTestIntersection);
  tcase_add_test(intersectionTests, noIntersection);
  tcase_add_test(intersectionTests, interLeavedIntersection); 
  suite_add_tcase(suite, intersectionTests);
  return suite;
}

/*************************** Main Test Function ************************/


typedef Suite *SuiteMaker(void);
static SuiteMaker *makers[] = {
  newIntSetSuite,
  addIntSetSuite,
  nElementsIntSetSuite,
  addMultipleIntSetSuite,
  isInIntSetSuite,
  iteratorSuite,
  sscanIntSetSuite,
  snprintIntSetSuite,
  unionIntSetSuite,
  intersectionIntSetSuite,
};


int
main(void)
{
  Suite *dummy = suite_create("IntSet Tests");
  SRunner *runner = srunner_create(dummy);
  for (int i = 0; i < sizeof(makers)/sizeof(makers[0]); i++) {
    srunner_add_suite(runner, makers[i]());
  }
  srunner_set_fork_status(runner, CK_NOFORK);
  srunner_run_all(runner, CK_NORMAL);
  int nFail = srunner_ntests_failed(runner);
  srunner_free(runner);
  return nFail != 0;
}
