#include "tidbitsp.h"
#include <cstdio>
#include <cerrno>
#include <dlfcn.h>
#include <fcntl.h>
static bool reject=false;
static int attempts=0;
extern "C" FILE * fdopen(int fd,const char *mode) {
  if (reject) { ++attempts; errno=ENOMEM; return NULL; }
  typedef FILE * (*Open)(int,const char *);
  static Open real = reinterpret_cast<Open>(dlsym(RTLD_NEXT,"fdopen"));
  return real ? real(fd,mode) : NULL;
}
extern "C" void free_std_fds(void);
static int descriptors() { int n=0; for(int fd=0;fd<256;++fd) if(fcntl(fd,F_GETFD)!=-1)++n; return n; }
int main() {
  const int before=descriptors(); reject=true;
  bool ok=coin_get_stdin()==NULL && coin_get_stdout()==NULL && coin_get_stderr()==NULL;
  reject=false; ok &= attempts==3 && descriptors()==before;
  free_std_fds();
  for(int fd=0;fd<3;++fd) ok &= fcntl(fd,F_GETFD)!=-1;
  ok &= coin_get_stdin()!=NULL && coin_get_stdout()!=NULL && coin_get_stderr()!=NULL;
  free_std_fds(); ok &= descriptors()==before;
  if(!ok) std::fprintf(stderr,"fdopen failure leaked or closed a standard descriptor\n");
  return ok ? 0 : 1;
}
