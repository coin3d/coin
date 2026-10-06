#include <Inventor/SoDB.h>
#include "fonts/fontlib_wrapper.h"
#include <atomic>
#include <thread>
#include <vector>
#include <cstdio>
int main() {
 SoDB::init();std::atomic<int> failures(0);
 for (int round=0;round<32;++round) {
  std::atomic<int> ready(0),acquired(0);std::atomic<bool> go(false),release(false);
  std::vector<int> ids(8,-1);std::vector<std::thread> threads;
  for(int t=0;t<8;++t) threads.emplace_back([&,t]{
   ++ready;while(!go.load())std::this_thread::yield();
   ids[t]=cc_flw_get_font_id_and_ref("defaultFont",12+round,0.7f,0.5f);
   ++acquired;while(!release.load())std::this_thread::yield();
   cc_flw_unref_font(ids[t]);
  });
  while(ready!=8)std::this_thread::yield();go=true;
  while(acquired!=8)std::this_thread::yield();
  for(int i=1;i<8;++i)if(ids[i]!=ids[0])++failures;
  release=true;for(auto &t:threads)t.join();
 }
 // Immediate acquisition/release overlaps lookup by other workers.
 std::vector<std::thread> threads;
 for(int i=0;i<8;++i)threads.emplace_back([&]{for(int n=0;n<5000;++n){
  int id=cc_flw_get_font_id_and_ref("defaultFont",24,0,0.5f);cc_flw_unref_font(id);
 }});
 for(auto &t:threads)t.join();SoDB::finish();
 std::printf("Font registry: 32 simultaneous-creation rounds + 40000 lifetime cycles, %d failures\n",failures.load());
 return failures?1:0;
}
