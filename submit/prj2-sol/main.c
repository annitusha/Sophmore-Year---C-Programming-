#include "int-set.h"
#include "int-set-strings.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
usage(const char *progName)
{
  fprintf(stderr, "usage:\t%s set SET: tests addIntSet()\n", progName);
  fprintf(stderr, "\t%s union SET SET: tests unionIntSet()\n", progName);
  fprintf(stderr, "\t%s intersection SET SET: tests intersectionIntSet()\n",
          progName);
  fprintf(stderr,
          "where SET is a brace-enclosed comma-separated set of ints.\n");
  exit(1);
}

static void *
getIntSet(const char *arg)
{
  void *set = sscanIntSet(arg, NULL);
  if (set == NULL) {
    fprintf(stderr, "invalid set literal \"%s\"\n", arg);
    exit(1);
  }
  return set;
}

static void
doSet(const char *arg, FILE *out)
{
  void *set = getIntSet(arg);
  int n = snprintIntSet(set, NULL, 0);
  char buf[n + 1];
  snprintIntSet(set, buf, n + 1);
  freeIntSet(set);
  fprintf(out, "%s\n", buf);
}

static void
doBinary(int isUnion, const char *arg1, const char *arg2, FILE *out)
{
  void *set1 = getIntSet(arg1);
  void *set2 = getIntSet(arg2);
  int ret = (isUnion ? unionIntSet : intersectionIntSet)(set1, set2);
  if (ret < 0) {
    fprintf(stderr, "%s failed: %s\n", isUnion ? "union" : "intersection",
            strerror(errno));
    exit(1);
  }
  int n = snprintIntSet(set1, NULL, 0);
  char buf[n + 1];
  snprintIntSet(set1, buf, n + 1);
  freeIntSet(set1);
  freeIntSet(set2);
  fprintf(out, "%s\n", buf);
}

int
main(int argc, const char *argv[])
{
  if (argc < 3) usage(argv[0]);
  const char *cmd = argv[1];
  if (strcmp(cmd, "set") == 0) {
    if (argc != 3) {
      fprintf(stderr, "expect SET argument for set command\n");
      exit(1);
    }
    doSet(argv[2], stdout);
  }
  else if (strcmp(cmd, "union") == 0 || strcmp(cmd, "intersection") == 0) {
    if (argc != 4) {
      fprintf(stderr, "expect two SET arguments for %s command\n", cmd);
      exit(1);
    }
    doBinary(strcmp(cmd, "union") == 0, argv[2], argv[3], stdout);
  }
}
