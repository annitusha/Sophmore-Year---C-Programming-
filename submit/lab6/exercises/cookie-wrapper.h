#ifndef COOKIE_WRAPPER_H_
#define COOKIE_WRAPPER_H_

/** Return cookie wrapped in some opaque wrapper */
int wrapCookie(int cookie);

/** Given wrapped cookie, use key to unwrap it and return unwrapped value */
int unwrapCookie(int wrapped, int key);


#endif //ifndef COOKIE_WRAPPER_H_
