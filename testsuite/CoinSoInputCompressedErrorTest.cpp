#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
static const unsigned char gzipdata[] = {31,139,8,0,0,0,0,0,2,3,83,246,204,43,75,205,43,201,47,82,8,51,210,51,84,72,44,78,206,204,228,10,78,45,72,44,74,4,9,86,215,114,1,0,161,8,251,40,34,0,0,0};
static const unsigned char bzdata[] = {66,90,104,57,49,65,89,38,83,89,207,105,93,46,0,0,2,223,128,0,16,72,1,48,0,0,32,9,0,42,33,221,10,32,0,34,38,129,136,12,141,5,26,50,6,141,50,52,246,88,134,102,119,73,163,9,113,245,133,101,112,139,135,65,196,113,239,139,185,34,156,40,72,103,180,174,151,0};

static bool check(const std::vector<unsigned char> &data,bool memory,bool expected,const char *label) {
  SoInput input; std::string path;
  if(memory) input.setBuffer(data.data(),data.size());
  else {
    const char *tmp=std::getenv("TMPDIR");path=std::string(tmp?tmp:"/tmp")+"/coin-read-error-XXXXXX";
    std::vector<char> name(path.begin(),path.end());name.push_back(0);
    int fd=mkstemp(name.data());if(fd<0)return false;path=name.data();
    FILE *file=fdopen(fd,"wb");if(!file){close(fd);unlink(path.c_str());return false;}
    bool wrote=fwrite(data.data(),1,data.size(),file)==data.size();fclose(file);
    if(!wrote || !input.openFile(path.c_str())){unlink(path.c_str());return false;}
  }
  char c=0;int count=0;while(input.read(c,FALSE))++count;
  bool ok=input.eof() && (input.hasReadError()!=FALSE)==expected && (expected || count>0);
  if(!ok) std::fprintf(stderr,"%s memory=%d count=%d eof=%d error=%d expected=%d\n",label,memory,count,input.eof(),input.hasReadError(),expected);
  const char clean[]="#Inventor V2.1 ascii\nSeparator {}\n";
  input.setBuffer(clean,sizeof(clean)-1);ok &= !input.hasReadError();
  while(input.read(c,FALSE)){} ok &= !input.hasReadError();
  input.closeFile();if(!path.empty())unlink(path.c_str());return ok;
}
int main(){
  SoDB::init();bool ok=true;
  for(bool memory:{false,true}) {
    std::vector<unsigned char> data(gzipdata,gzipdata+sizeof(gzipdata));
    ok &= check(data,memory,false,"valid gzip");
    data.resize(sizeof(gzipdata)-4);ok &= check(data,memory,true,"truncated gzip trailer");
    data.assign(gzipdata,gzipdata+sizeof(gzipdata)/2);ok &= check(data,memory,true,"truncated gzip body");
    data.assign(gzipdata,gzipdata+sizeof(gzipdata));data[data.size()-8]^=1;
    ok &= check(data,memory,true,"gzip CRC error");
  }
  std::vector<unsigned char> data(bzdata,bzdata+sizeof(bzdata));
  ok &= check(data,false,false,"valid bzip2");data.resize(sizeof(bzdata)/2);
  ok &= check(data,false,true,"truncated bzip2");
  SoDB::finish();return ok?0:1;
}
