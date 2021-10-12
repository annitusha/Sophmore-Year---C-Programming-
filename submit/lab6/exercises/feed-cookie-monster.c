#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cookie-wrapper.h"

#define COOKIE_FILE  "../.cookie"

static int getCookie() {
  FILE *cookieFile = fopen(COOKIE_FILE, "r");
  if (!cookieFile) {
    fprintf(stderr, "cannot read %s: %s\n", COOKIE_FILE, strerror(errno));
    exit(1);
  }
  int cookie = -1;
  int n = fscanf(cookieFile, "0x%x", &cookie);
  fclose(cookieFile);
  if (n != 1) {
    fprintf(stderr, "unable to scan cookie file\n");
    exit(1);
  }
  return cookie;
}

static int getUnwrapKey() {
  int key;
  int n = 0;
  while (n != 1) {
    printf("enter cookie unwrap key in hex: ");
    fflush(stdout);
    n = scanf("%x", &key);
  }
  return key;
}

int main() {

  printf("Me want cookie!\n");
  const int cookie = getCookie();
  int wrapped = wrapCookie(cookie);
  int key = getUnwrapKey();
  int gotIt = unwrapCookie(wrapped, key) == cookie;
  printf(gotIt ? "😂 CHOMP!!! CHOMP!!!\n" : "🥶 AARGH YUCK!!\n");
  return !!gotIt;

}
