#include "runningmaxmin.h"

#include <cmath>
#include <cstring>
#include <ctime>
#include <iomanip>

bool compare(std::vector<floattype> & a, std::vector<floattype> & b) {
    if (a.size() != b.size())
        return false;
    for (uint k = 0; k < a.size(); ++k)
        if (a[k] != b[k])
            return false;
    return true;
}
bool compare(minmaxfilter & a, minmaxfilter & b) {
    return static_cast<int>((compare(a.getmaxvalues(), b.getmaxvalues())) &
                            static_cast<int>(compare(a.getminvalues(),
                                                     b.getminvalues()))) != 0;
}

void display(minmaxfilter & a) {
    display(a.getmaxvalues());
    display(a.getminvalues());
}

void test(std::vector<floattype> data, uint width = 3) {

    slowmaxmin A(data, width);
    lemiremaxmin C(data, width);
    lemiremaxminwrap Cw(data, width);
    simplelemiremaxmin G(data, width);
    GilKimmel D(data, width);
    vanHerkGilWermanmaxmin B(data, width);
    lemirebitmapmaxmin E(data, width);
    monowedgewrap M(data,width);
    if (!compare(A, M)) {
        std::cout << "array " << std::endl;
        display(data);
        std::cout << "solution " << std::endl;
        display(A);
        std::cout << "monowedgewrap " << std::endl;
        display(M);
    }
    if (!compare(A, Cw)) {
        std::cout << "array " << std::endl;
        display(data);
        std::cout << "solution " << std::endl;
        display(A);
        std::cout << "lemirew " << std::endl;
        display(Cw);
    }

    if (!compare(A, D)) {
        std::cout << "array " << std::endl;
        display(data);
        std::cout << "solution " << std::endl;
        display(A);
        std::cout << "lemiremaxmin " << std::endl;
        display(D);
    }
    if (!compare(A, C)) {
        std::cout << "array " << std::endl;
        display(data);
        std::cout << "solution " << std::endl;
        display(A);
        std::cout << "lemiremaxmin " << std::endl;
        display(C);
    }
    if (!compare(A, B)) {
        std::cout << "array " << std::endl;
        display(data);
        std::cout << "solution " << std::endl;
        display(A);
        std::cout << "vanHerk " << std::endl;
        display(B);
    }
    assert(compare(A, Cw));

    assert(compare(A, C));
    assert(compare(A, B));
    assert(compare(A, D));
    assert(compare(A, G));
    assert(compare(A, E));
}

void unit() {
    uint size = 20;
    std::vector<floattype> data(size);
    for (uint k = 0; k < size; ++k)
        data[k] = size - k;
    test(data, 6);
    test(data, 2);
    test(data, 3);
    test(data, 4);
    test(data, 5);
    for (uint k = 0; k < size; ++k)
        data[k] = k;
    test(data, 2);
    test(data, 3);
    test(data, 4);
    test(data, 5);
    for (uint j = 0; j < 1000; ++j) {
        for (uint k = 0; k < size; ++k)
            data[k] = rand();
        test(data, 2);
        test(data, 3);
        test(data, 4);
        test(data, 5);
    }
}

int main() {
  unit();
  std::cout << "Code appears ok." << std::endl;
  return 0;
}
