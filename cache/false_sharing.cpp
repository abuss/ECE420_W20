// To compile:
//   g++ -O0 -Wall -o false_sharing false_sharing.cpp -lpthread

#include <iostream>
#include <ctime>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>  
  
constexpr double M=8e8;
  
//Shared_Work holds the work need to be done
//part1() and part2() separates the work half-and-half
//n1 and n2 are data items
struct Shared_Work 
{
  Shared_Work()
    : n1(1),n2(1),flag1(0),flag2(0)
  { }

  void part1() {
    for(; n1<M; n1+=n1%3);
    std::cout << "done p1\n";
    flag1=1;
    cond.notify_one();
  }

  void part2() {
    for(; n2<M; n2+=n2%3);
    std::cout << "done p2\n";
    flag2=1;
    cond.notify_one();
  }

  long int result() {
    std::unique_lock<std::mutex> lk(mtx);
    while(flag1==0||flag2==0)
        cond.wait(lk);
    return n1+n2;
  }
  
private:
  std::mutex mtx;
  std::condition_variable cond;
  long int n1;
  // char chuck[64];//cache line seperation
  long int n2;
  bool flag1;
  bool flag2;
};
  

//single thread work to do
void one_thread() {
  long int n1=1, n2=1;
  for(; n1<M; n1+=n1%3);
  for(; n2<M; n2+=n2%3);
  std::cout << n1+n2 << std::endl;
}
  
     
int main(int argc, char* argv[]) 
{  
  time_t start, end;
  double diff;
  
  Shared_Work sw;

  time(&start);
  std::thread t1( [&]{sw.part1();} );
  std::thread t2( [&]{sw.part2();} );
  t1.detach();//release the ownership to C++ runtime library.
  t2.detach();//release the ownership to C++ runtime library.
  
  std::cout << sw.result() << std::endl;
  time(&end);
  diff=difftime(end, start);
  std::cout << diff << " seconds elapsed for 2 threads calculation." << std::endl;

  time(&start);
  one_thread();
  time(&end);
  diff=difftime(end, start);
  std::cout << diff << " seconds elapsed for 1 thread calculation." << std::endl;  
}