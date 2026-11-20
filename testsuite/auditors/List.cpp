#include "Support.h"
#include <climits>
#include <stdexcept>

static void content(const SoAuditorList & l, int * p, int n) {
  CHECK(l.getLength()==n);
  for(int i=0;i<n;++i) {
    CHECK(l.getObject(i)==&p[i]);
    CHECK(l.getType(i)==(i%2?SoNotRec::CONTAINER:SoNotRec::PARENT));
  }
}
static void behavior() {
  int p[12]; SoAuditorList l;
  for(int i=0;i<8;++i) l.append(&p[i],i%2?SoNotRec::CONTAINER:SoNotRec::PARENT);
  l.remove(&p[11],SoNotRec::PARENT);
  l.remove(&p[0],SoNotRec::CONTAINER);
  content(l,p,8);
  // Valid replacement and removal must work with every allocation disabled.
  allocation::arm(0);
  try {
    l.set(3,&p[9],SoNotRec::PARENT);
    l.remove(5);
  } catch(...) { allocation::disarm(); CHECK(false); }
  allocation::disarm();
  CHECK(l.getLength()==7 && l.getObject(3)==&p[9]);
  CHECK(l.getType(3)==SoNotRec::PARENT);
  CHECK(l.getObject(4)==&p[4] && l.getObject(5)==&p[6] && l.getObject(6)==&p[7]);
  l.append(&p[0],SoNotRec::PARENT); l.append(&p[0],SoNotRec::PARENT);
  CHECK(occurrences(l,&p[0],SoNotRec::PARENT)==3);
  l.remove(&p[0],SoNotRec::PARENT);
  CHECK(occurrences(l,&p[0],SoNotRec::PARENT)==2);
  unlocked(false);
}
static void bounds(const char * operation,int index) {
  int p[4]; SoAuditorList l;
  for(int i=0;i<4;++i) l.append(&p[i],i%2?SoNotRec::CONTAINER:SoNotRec::PARENT);
  bool caught=false;
  try {
    if(!std::strcmp(operation,"set")) l.set(index,&p[0],SoNotRec::PARENT);
    else if(!std::strcmp(operation,"remove")) l.remove(index);
    else if(!std::strcmp(operation,"object")) (void)l.getObject(index);
    else if(!std::strcmp(operation,"type")) (void)l.getType(index);
    else CHECK(false);
  } catch(const std::out_of_range &) { caught=true; }
  CHECK(caught); content(l,p,4); unlocked(false);
}
static void empty() {
  int p; SoAuditorList l;
  l.remove(&p,SoNotRec::PARENT);
  CHECK(l.getLength()==0);
  for(int operation=0;operation<4;++operation) {
    bool caught=false;
    try {
      if(operation==0) l.set(0,&p,SoNotRec::PARENT);
      else if(operation==1) l.remove(0);
      else if(operation==2) (void)l.getObject(0);
      else (void)l.getType(0);
    } catch(const std::out_of_range &) { caught=true; }
    CHECK(caught);
    CHECK(l.getLength()==0);
  }
  unlocked(false);
}
static void endpoints() {
  int p[5]; SoAuditorList l;
  for(int i=0;i<3;++i) l.append(&p[i],SoNotRec::PARENT);
  l.set(0,&p[3],SoNotRec::CONTAINER);
  l.set(2,&p[4],SoNotRec::CONTAINER);
  CHECK(l.getObject(0)==&p[3] && l.getType(0)==SoNotRec::CONTAINER);
  CHECK(l.getObject(1)==&p[1] && l.getType(1)==SoNotRec::PARENT);
  CHECK(l.getObject(2)==&p[4] && l.getType(2)==SoNotRec::CONTAINER);
  l.remove(2);
  l.remove(0);
  CHECK(l.getLength()==1 && l.getObject(0)==&p[1] && l.getType(0)==SoNotRec::PARENT);
  l.remove(0);
  CHECK(l.getLength()==0);
  l.append(&p[0],SoNotRec::PARENT);
  CHECK(l.getLength()==1 && l.getObject(0)==&p[0] && l.getType(0)==SoNotRec::PARENT);
  unlocked(false);
}
static void failure(long n,int length) {
  int p[80];
  {
    SoAuditorList l;
    for(int i=0;i<length;++i) l.append(&p[i],i%2?SoNotRec::CONTAINER:SoNotRec::PARENT);
    bool caught=false;
    allocation::arm(n);
    try { l.append(&p[length],length%2?SoNotRec::CONTAINER:SoNotRec::PARENT); }
    catch(const std::bad_alloc &) { caught=true; }
    allocation::disarm();
    if(n==0 && (length==4 || length==8 || length==16 || length==32)) CHECK(caught);
    content(l,p,length+(caught?0:1));
    unlocked(false);
    if(caught) l.append(&p[length],length%2?SoNotRec::CONTAINER:SoNotRec::PARENT);
    content(l,p,length+1);
  }
}
int main(int argc,char ** argv) {
  SoDB::init();
  if(argc==1) behavior();
  else if(!std::strcmp(argv[1],"empty")) { CHECK(argc==2); empty(); }
  else if(!std::strcmp(argv[1],"endpoints")) { CHECK(argc==2); endpoints(); }
  else if(!std::strcmp(argv[1],"fail")) { CHECK(argc==4); failure(std::strtol(argv[2],NULL,10),std::atoi(argv[3])); }
  else { CHECK(argc==3); bounds(argv[1],std::atoi(argv[2])); }
  SoDB::finish();
}
