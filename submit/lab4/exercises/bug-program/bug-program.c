#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** A struct used to form a linear chain of key-value pairs. */
typedef struct KeyValueStruct {
  const char *key;       /** string key */
  int value;             /** integer value */
  struct KeyValueStruct *succ; /** next key-value in chain */
} KeyValueNode;

/** Given some chain keyValues of key-value struct's, add the
 *  key-value (k, v) at head of chain keyValues and return the
 *  resulting chain.
 */
static KeyValueNode *
add_key_value(KeyValueNode *keyValues, const char *k, int v)
{
  //allocate storage for new KeyValue struct
  KeyValueNode *kv = malloc(sizeof(KeyValueNode *));

  //allocate storage for string pointed to by k
  char *s = malloc(strlen(k));

  if (kv == NULL || s == NULL) { //check if allocations succeeded
    fprintf(stderr, "malloc failure: %s\n", strerror(errno));
    exit(1);
  }
  strcpy(s, k);  //copy string k into newly allocated memory pointed to by s

  //initialize fields of *kv.
  kv->key = s;
  kv->value = v;
  kv->succ = keyValues;

  return kv;
}

/** Free all the key-value's in keyValues. */
static void
free_key_values(KeyValueNode *keyValues)
{
  //go thru chain of keyValues
  for (KeyValueNode *p = keyValues; p != NULL; p = p->succ) {
    free(p); //free KeyValue struct
  }
}

/** Make key-values for all the words in the first verse of Carroll's
 *  Jabberwocky with value being the index of the word.
 */
static KeyValueNode *
make_key_values(void) {
  const char *keys[] = {
    "twas", "brillig", "and", "the", "slithy", "toves",
    "did", "gyre", "and", "gimble", "in", "the", "wabe",
    "all", "mimsy", "were", "the", "borogoves",
    "and", "the", "mome", "raths", "outgrabe",
  };
  KeyValueNode *p = NULL;
  for (int i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
    p = add_key_value(p, keys[i], i);
  }
  return p;
}

/** Print out all the words in the first verse of Carroll's Jabberwocky
 *  paired with their index in the verse.
 */
int main() {
  KeyValueNode *keyValues = make_key_values();
  for (KeyValueNode *p = keyValues; p != NULL; p = p->succ) {
    printf("%s: %d\n", p->key, p->value);
  }
  free_key_values(keyValues);
  return 0;
}
