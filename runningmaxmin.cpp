#include <cmath>
#include <iostream>
#include <iomanip>

#include <stdlib.h>
#include <time.h>
#include "runningmaxmin.h"

bool compare(vector<floattype> &a, vector<floattype> &b) {
    if (a.size() != b.size())
        return false;
    for (uint k = 0; k < a.size(); ++k)
        if (a[k] != b[k])
            return false;
    return true;
}
bool compare(minmaxfilter & a, minmaxfilter & b) {
    return compare(a.getmaxvalues(), b.getmaxvalues()) & compare(
               a.getminvalues(), b.getminvalues());
}

void display(minmaxfilter & a) {
    display(a.getmaxvalues());
    display(a.getminvalues());
}

void test(vector<floattype> data, uint width = 3) {

    slowmaxmin A(data, width);
    lemiremaxmin C(data, width);
    lemiremaxminwrap Cw(data, width);
    simplelemiremaxmin G(data, width);
    GilKimmel D(data, width);
    vanHerkGilWermanmaxmin B(data, width);
    lemirebitmapmaxmin E(data,width);
    if (!compare(A, Cw)) {
        cout << "array " << endl;
        display(data);
        cout << "solution " << endl;
        display(A);
        cout << "lemirew " << endl;
        display(Cw);
    }

    if (!compare(A, D)) {
        cout << "array " << endl;
        display(data);
        cout << "solution " << endl;
        display(A);
        cout << "lemiremaxmin " << endl;
        display(D);
    }
    if (!compare(A, C)) {
        cout << "array " << endl;
        display(data);
        cout << "solution " << endl;
        display(A);
        cout << "lemiremaxmin " << endl;
        display(C);
    }
    if (!compare(A, B)) {
        cout << "array " << endl;
        display(data);
        cout << "solution " << endl;
        display(A);
        cout << "vanHerk " << endl;
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
    vector<floattype> data(size);
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

vector<floattype> getwhite(uint size) {
    vector<floattype> data(size);
    for (uint k = 0; k < size; ++k)
        data[k] = (1.0 * rand() / (RAND_MAX)) - 0.5;
    return data;
}

vector<floattype> getrandomwalk(uint size) {
    vector<floattype> data(size);
    data[0] = 0.0;
    for (uint k = 1; k < size; ++k)
        data[k] = (1.0 * rand() / (RAND_MAX)) - 0.5 + data[k - 1];
    return data;
}

vector<floattype> getcin() {
    float val;
    cin >> val;
    vector<floattype> v;
    while (cin) {
        v.push_back(val);
        cin >> val;
    }
    cout << "# Read " << v.size() << " data points. " << endl;
    return v;
}

void compareallalgos(vector<floattype> & data, vector<double> & timings,
                     uint width, bool doslow) {
    if (timings.size() < 7)
        timings = vector<double> (7, 0.0);
    clock_t start, finish;
    start = clock();
    if (doslow)
        slowmaxmin A(data, width);
    finish = clock();
    timings[0] += (double) (finish - start) / CLOCKS_PER_SEC;
    start = clock();
    vanHerkGilWermanmaxmin B(data, width);
    finish = clock();
    timings[1] += (double) (finish - start) / CLOCKS_PER_SEC;
    start = clock();
    lemiremaxmin C(data, width);
    finish = clock();
    timings[2] += (double) (finish - start) / CLOCKS_PER_SEC;
    start = clock();
    GilKimmel D(data, width);
    finish = clock();
    timings[3] += (double) (finish - start) / CLOCKS_PER_SEC;
    start = clock();
    if(width <  sizeof(unsigned long)*8) lemirebitmapmaxmin BL(data, width);
    finish = clock();
    timings[4] += (double) (finish - start) / CLOCKS_PER_SEC;
    start = clock();
    simplelemiremaxmin F(data, width);
    finish = clock();
    timings[5] += (double) (finish - start) / CLOCKS_PER_SEC;
    start = clock();
    lemiremaxminwrap Cw(data, width);
    finish = clock();
    timings[6] += (double) (finish - start) / CLOCKS_PER_SEC;

}

void process(vector<floattype> & data, uint width = 30, uint times = 1,
             bool doslow = true) {
    vector<double> timings;
    for (uint i = 0; i < times; ++i) {
        compareallalgos(data, timings, width, doslow);
    }
    cout << "#\tslow\t\tvanHerk\t\tlemire\tsimplelemire\t\tgilkimmel" << endl;
    cout << "\t" << timings[0] << "\t" << timings[1] << "\t" << timings[2]
         << "\t" << timings[5] << "\t\t" << timings[3] << endl;
}

void timings(uint width = 50, uint size = 10000, uint times = 500,
             bool doslow = true) {
    vector<double> timings;
    for (uint i = 0; i < times; ++i) {
        vector<floattype> data = getwhite(size);
        compareallalgos(data, timings, width, doslow);
    }
    cout <<  setw(15) << "slow";
    cout <<  setw(15) << "vanHerk";
    cout <<  setw(15) << "lemire";
    cout <<  setw(15) << "gilkimmel";
    cout <<  setw(15) << "bitmap";
    cout <<  setw(15) << "simplelemire";
    cout <<  setw(15) << "lemirew";
    cout << endl;
    for(int i = 0; i <= 6; ++i ) {
        cout<< setw(15) << timings[i] ;
    }
    cout << endl;

}

void walktimings(uint width = 50, uint size = 10000, uint times = 500,
                 bool doslow = true) {
    vector<double> timings;
    for (uint i = 0; i < times; ++i) {
        vector<floattype> data = getrandomwalk(size);
        compareallalgos(data, timings, width, doslow);
    }
    cout <<  setw(15) << "slow";
    cout <<  setw(15) << "vanHerk";
    cout <<  setw(15) << "lemire";
    cout <<  setw(15) << "gilkimmel";
    cout <<  setw(15) << "bitmap";
    cout <<  setw(15) << "simplelemire";
    cout <<  setw(15) << "lemirew";
    cout << endl;
    for(int i = 0; i <= 6; ++i ) {
        cout<< setw(15) << timings[i] ;
    }
    cout << endl;

}

void sinetimings(uint width = 50, uint size = 10000, floattype period = 500.0,
                 uint times = 500, bool doslow = true) {
    vector<double> timings;
    for (uint i = 0; i < times; ++i) {
        vector<floattype> data(size);
        for (uint j = 0; j < size; ++j)
            data[j] = sin(2 * M_PI * j / period);
        compareallalgos(data, timings, width, doslow);
    }
    cout <<  setw(15) << "slow";
    cout <<  setw(15) << "vanHerk";
    cout <<  setw(15) << "lemire";
    cout <<  setw(15) << "gilkimmel";
    cout <<  setw(15) << "bitmap";
    cout <<  setw(15) << "simplelemire";
    cout <<  setw(15) << "lemirew";
    cout << endl;
    for(int i = 0; i <= 6; ++i ) {
        cout<< setw(15) << timings[i] ;
    }
    cout << endl;

}

void timingsline(vector<floattype> data, uint width = 30, bool doslow = false) {
    cout << " width = " << width << endl;
    clock_t start, finish;
    time_t time(time_t *t);
    start = clock();
    if (doslow)
        for (uint i = 0; i < 30; ++i)
            slowmaxmin A(data, width);
    finish = clock();
    cout << "slow = " << (double) (finish - start) / CLOCKS_PER_SEC << endl;
    start = clock();
    for (uint i = 0; i < 30; ++i)
        vanHerkGilWermanmaxmin B(data, width);
    finish = clock();
    cout << "vanHerk = " << (double) (finish - start) / CLOCKS_PER_SEC << endl;
    start = clock();
    for (uint i = 0; i < 30; ++i)
        lemiremaxmin C(data, width);
    finish = clock();
    cout << "lemire = " << (double) (finish - start) / CLOCKS_PER_SEC << endl;
    cout << "------------" << endl;
}

/**
 * use in conjunction with

 | awk '{print $2}'
 */
int main(int params, char ** args) {
    unit();
    int whitesize = 0;
    int walksize = 0;
    int times = 1;
    int sinesize = 0;
    floattype sineperiod = 0.0;
    int windowbegin = 10;
    int windowend = 11;
    bool doslow = true;
    bool cininput = false;
    vector<floattype> data(0);
    for (int i = 1; i < params; ++i) {
        if (strcmp(args[i], "--skipslow") == 0) {
            doslow = false;
        }
        if (strcmp(args[i], "--pipedata") == 0) {
            cininput = true;
        }
        if (strcmp(args[i], "--unit") == 0) {
            cout << "OK " << endl;
            return 0;
        }
        if (strcmp(args[i], "--window") == 0) {
            if (params - i > 1) {
                windowbegin = atoi(args[++i]);
                windowend = windowbegin + 1;
            } else {
                cerr << "--window expects an integer (length)" << endl;
                return -1;
            }
            continue;
        }
        if (strcmp(args[i], "--windowrange") == 0) {
            if (params - i > 2) {
                windowbegin = atoi(args[++i]);
                windowend = atoi(args[++i]) + 1;
            } else {
                cerr << "--windowrange expects two integers (length)" << endl;
                return -1;
            }
            continue;
        }
        if (strcmp(args[i], "--white") == 0) {
            if (params - i > 1)
                whitesize = atoi(args[++i]);
            else {
                cerr << "--white expects an integer (length)" << endl;
                return -1;
            }
            continue;
        }
        if (strcmp(args[i], "--walk") == 0) {
            if (params - i > 1)
                walksize = atoi(args[++i]);
            else {
                cerr << "--walk expects an integer (length)" << endl;
                return -1;
            }
            continue;
        }
        if (strcmp(args[i], "--sine") == 0) {
            if (params - i > 1)
                sinesize = atoi(args[++i]);
            else {
                cerr << "--sine expects an integer (length)" << endl;
                return -1;
            }
            if (params - i > 1)
                sineperiod = atof(args[++i]);
            else {
                cerr
                        << "--sine expects an integer (length) followed by a float (freq) "
                        << endl;
                return -1;
            }
            continue;
        }
        if (strcmp(args[i], "--times") == 0) {
            if (params - i > 1)
                times = atoi(args[++i]);
            else {
                cerr << "--times expects an integer" << endl;
                return -1;
            }
            continue;
        }
    }
    for (int window = windowbegin; window < windowend; ++window) {
        if (whitesize > 0) {
            cout << "# window = " << window << " whitesize = " << whitesize
                 << " times = " << times << " doslow = " << doslow << endl;
            assert(window + 1 < whitesize);
            timings(window, whitesize, times, doslow);
        } else if (walksize > 0) {
            cout << "# window = " << window << " walksize = " << walksize
                 << " times = " << times << " doslow = " << doslow << endl;
            walktimings(window, walksize, times, doslow);

        } else if (sinesize > 0) {
            cout << "# window = " << window << " sinesize = " << sinesize
                 << " sine period " << sineperiod << " times = " << times
                 << " doslow = " << doslow << endl;
            assert(window + 1 < sinesize);
            sinetimings(window, sinesize, sineperiod, times, doslow);
        } else {
            if ((data.size() == 0) && cininput) {
                data = getcin();
                cout << "# window = " << window << " times = " << times
                     << " doslow = " << doslow << endl;
                process(data, window, times, doslow);
            } else {
                cout<<"Generating sine waves."<<endl;
                for(uint width = 3; width <=100; ++width) {
                    cout << "Using a min/max width of " << width << endl;
                    sinetimings(width);
                }
            }
        }
    }
    return 0;

}
