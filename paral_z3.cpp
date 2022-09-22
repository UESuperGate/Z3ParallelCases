#include <z3++.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <string>
#include <vector>
#include <cstring>
#include <map>
#include <fstream>
#include <algorithm>

#include "ThreadPool2.h"

ThreadPool *tp = nullptr;

const uint64_t kUsToS = 1000000;

uint64_t getTimeStamp() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * kUsToS + tv.tv_usec;
}

void initialization() {
    uint64_t threadNum = 4;
    auto *maxThreadLimit = getenv("SYMCC_MAX_THREAD");
    if (maxThreadLimit != nullptr) {
        threadNum = std::stoul(maxThreadLimit);
    }
    tp = new ThreadPool(threadNum);
}

void do_solving_handler(std::string c1,
                        std::string c2,
                        int solving_id) {
    z3::context ctx;
    z3::solver sv(ctx, "QF_BV");
    z3::solver sub_sv(ctx, "QF_BV");

    sv.from_file(c1.c_str());
    sub_sv.from_file(c2.c_str());

    z3::check_result sat = sv.check();
    std::cerr << "====================== " << c1 << " ===========================\n";
    std::cerr << sv.statistics() << std::endl;
    std::cerr << sat << std::endl;

    if (sat != z3::sat) {
        sat = sub_sv.check();
        std::cerr << "====================== " << c2 << " ===========================\n";
        std::cerr << sub_sv.statistics() << std::endl;
    }
    
}


int main(int argc, char ** argv) {


    initialization();

    tp->enqueue(0, do_solving_handler, "00000000000000001835", "sub_00000000000000001835", 1835);
    tp->enqueue(0, do_solving_handler, "00000000000000001889", "sub_00000000000000001889", 1889);
    tp->enqueue(0, do_solving_handler, "00000000000000001908", "sub_00000000000000001908", 1908);
    tp->enqueue(0, do_solving_handler, "00000000000000002074", "sub_00000000000000002074", 2074);
    tp->~ThreadPool();

    // single_solving("/home1/qingyu/benchmark/program-set/zlib/new/00000000000000001835");        // unsat
    // single_solving("/home1/qingyu/benchmark/program-set/zlib/new/sub_00000000000000001835");    // sat
    // single_solving("/home1/qingyu/benchmark/program-set/zlib/new/00000000000000001889");        // sat
    // single_solving("/home1/qingyu/benchmark/program  -set/zlib/new/00000000000000001908");        // unsat
    // single_solving("/home1/qingyu/benchmark/program-set/zlib/new/sub_00000000000000001908");    // sat
    // single_solving("/home1/qingyu/benchmark/program-set/zlib/new/00000000000000002074");        // unsat
    // single_solving("/home1/qingyu/benchmark/program-set/zlib/new/sub_00000000000000002074");    // sat

}
