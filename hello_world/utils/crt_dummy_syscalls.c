#include <errno.h>
#include <sys/stat.h>

//
// The Newlib that comes with gcc-arm-12.2.Rel1 introduces a new linker warning, for example:
//   ../lib/gcc/arm-none-eabi/12.2.1/thumb/v7e-m+dp/hard\libc.a(libc_a-closer.o): in function `_close_r':
//   closer.c:(.text._close_r+0xc): warning: _close is not implemented and will always fail
//
// The SysCall stubs implemented in libnosys.a are decorated with a special linker section which causes this warning.
// As a workaround, we implement our own dummy stubs below.
//
// See also https://sourceware.org/newlib/libc.html#Stubs
//


int _close(int file)
{
    (void)(file);
    return -1;
}

int _fstat(int file, struct stat *st) {
    (void)(file);
    st->st_mode = S_IFCHR;
    return 0;
}

int _getpid(void) {
  return 1;
}

int _isatty(int file) {
    (void)(file);
    return 1;
}

int _kill(int pid, int sig) {
    (void)(pid);
    (void)(sig);
    errno = EINVAL;
    return -1;
}

int link(char *old, char *new) {
    (void)(old);
    (void)(new);
    errno = EMLINK;
    return -1;
}

int _lseek(int file, int ptr, int dir) {
    (void)(file);
    (void)(ptr);
    (void)(dir);
    return -1;
}

int open(const char *name, int flags, int mode) {
    (void)(name);
    (void)(flags);
    (void)(mode);
    return -1;
}

int _read(int file, char *ptr, int len) {
    (void)(file);
    (void)(ptr);
    (void)(len);
    return -1;
}

int _stat(char *file, struct stat *st) {
    (void)(file);
    st->st_mode = S_IFCHR;
    return 0;
}

int unlink(char *name) {
    (void)(name);
    errno = ENOENT;
    return -1;
}

int _write(int file, char *ptr, int len) {
    (void)(file);
    (void)(ptr);
    (void)(len);
    return -1;
}



/*** EOF ***/
