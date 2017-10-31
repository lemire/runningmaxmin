#include "runningmaxmin.h"

#include <cmath>
#include <cstring>
#include <ctime>
#include <iomanip>


std::vector<floattype> getwhite(uint size) {
    std::vector<floattype> data(size);
    for (uint k = 0; k < size; ++k)
        data[k] = (1.0 * rand() / (RAND_MAX)) - 0.5;
    return data;
}

std::vector<floattype> getrandomwalk(uint size) {
    std::vector<floattype> data(size);
    data[0] = 0.0;
    for (uint k = 1; k < size; ++k)
        data[k] = (1.0 * rand() / (RAND_MAX)) - 0.5 + data[k - 1];
    return data;
}

std::vector<floattype> getcin() {
    float val;
    std::cin >> val;
    std::vector<floattype> v;
    while (std::cin) {
        v.push_back(val);
        std::cin >> val;
    }
    std::cout << "# Read " << v.size() << " data points. " << std::endl;
    return v;
}

void compareallalgos(std::vector<floattype> & data,
                     std::vector<double> & timings, uint width, bool doslow) {
    if (timings.size() < 8)
        timings = std::vector<double>(8, 0.0);
    clock_t start, finish;
    start = clock();
    if (doslow)
        slowmaxmin A(data, width);
    finish = clock();
    timings[0] += static_cast<double>(finish - start) / CLOCKS_PER_SEC;
    start = clock();
    vanHerkGilWermanmaxmin B(data, width);
    finish = clock();
    timings[1] += static_cast<double>(finish - start) / CLOCKS_PER_SEC;
    start = clock();
    lemiremaxmin C(data, width);
    finish = clock();
    timings[2] += static_cast<double>(finish - start) / CLOCKS_PER_SEC;
    start = clock();
    GilKimmel D(data, width);
    finish = clock();
    timings[3] += static_cast<double>(finish - start) / CLOCKS_PER_SEC;
    start = clock();
    if (width < sizeof(unsigned long) * 8)
        lemirebitmapmaxmin BL(data, width);
    finish = clock();
    timings[4] += static_cast<double>(finish - start) / CLOCKS_PER_SEC;
    start = clock();
    simplelemiremaxmin F(data, width);
    finish = clock();
    timings[5] += static_cast<double>(finish - start) / CLOCKS_PER_SEC;
    start = clock();
    lemiremaxminwrap Cw(data, width);
    finish = clock();
    timings[6] += static_cast<double>(finish - start) / CLOCKS_PER_SEC;
    start = clock();
    monowedgewrap Mw(data, width);
    finish = clock();
    timings[7] += static_cast<double>(finish - start) / CLOCKS_PER_SEC;
}

void process(std::vector<floattype> & data, uint width = 30, uint times = 1,
             bool doslow = true) {
    std::vector<double> timings;
    for (uint i = 0; i < times; ++i) {
        compareallalgos(data, timings, width, doslow);
    }
    std::cout << "#\tslow\t\tvanHerk\t\tlemire\tsimplelemire\t\tgilkimmel"
              << std::endl;
    std::cout << "\t" << timings[0] << "\t" << timings[1] << "\t" << timings[2]
              << "\t" << timings[5] << "\t\t" << timings[3] << std::endl;
}

void timings(uint width = 50, uint size = 10000, uint times = 500,
             bool doslow = true) {
    std::vector<double> timings;
    for (uint i = 0; i < times; ++i) {
        std::vector<floattype> data = getwhite(size);
        compareallalgos(data, timings, width, doslow);
    }
    std::cout << std::setw(15) << "slow";
    std::cout << std::setw(15) << "vanHerk";
    std::cout << std::setw(15) << "lemire";
    std::cout << std::setw(15) << "gilkimmel";
    std::cout << std::setw(15) << "bitmap";
    std::cout << std::setw(15) << "simplelemire";
    std::cout << std::setw(15) << "lemirew";
    std::cout << std::setw(15) << "monowedge";
    std::cout << std::endl;
    for (int i = 0; i <= 7; ++i) {
        std::cout << std::setw(15) << timings[i];
    }
    std::cout << std::endl;
}

void walktimings(uint width = 50, uint size = 10000, uint times = 500,
                 bool doslow = true) {
    std::vector<double> timings;
    for (uint i = 0; i < times; ++i) {
        std::vector<floattype> data = getrandomwalk(size);
        compareallalgos(data, timings, width, doslow);
    }
    std::cout << std::setw(15) << "slow";
    std::cout << std::setw(15) << "vanHerk";
    std::cout << std::setw(15) << "lemire";
    std::cout << std::setw(15) << "gilkimmel";
    std::cout << std::setw(15) << "bitmap";
    std::cout << std::setw(15) << "simplelemire";
    std::cout << std::setw(15) << "lemirew";
    std::cout << std::setw(15) << "monowedge";
    std::cout << std::endl;
    for (int i = 0; i <= 7; ++i) {
        std::cout << std::setw(15) << timings[i];
    }
    std::cout << std::endl;
}

void sinetimings(uint width = 50, uint size = 10000, floattype period = 500.0,
                 uint times = 500, bool doslow = true) {
    std::vector<double> timings;
    for (uint i = 0; i < times; ++i) {
        std::vector<floattype> data(size);
        for (uint j = 0; j < size; ++j)
            data[j] = sin(2 * M_PI * j / period);
        compareallalgos(data, timings, width, doslow);
    }
    std::cout << std::setw(15) << "slow";
    std::cout << std::setw(15) << "vanHerk";
    std::cout << std::setw(15) << "lemire";
    std::cout << std::setw(15) << "gilkimmel";
    std::cout << std::setw(15) << "bitmap";
    std::cout << std::setw(15) << "simplelemire";
    std::cout << std::setw(15) << "lemirew";
    std::cout << std::setw(15) << "monowedge";
    std::cout << std::endl;
    for (int i = 0; i <= 7; ++i) {
        std::cout << std::setw(15) << timings[i];
    }
    std::cout << std::endl;
}

void timingsline(std::vector<floattype> data, uint width = 30,
                 bool doslow = false) {
    std::cout << " width = " << width << std::endl;
    clock_t start, finish;
    time_t time(time_t * t);
    start = clock();
    if (doslow)
        for (uint i = 0; i < 30; ++i)
            slowmaxmin A(data, width);
    finish = clock();
    std::cout << "slow = "
              << static_cast<double>(finish - start) / CLOCKS_PER_SEC
              << std::endl;
    start = clock();
    for (uint i = 0; i < 30; ++i)
        vanHerkGilWermanmaxmin B(data, width);
    finish = clock();
    std::cout << "vanHerk = "
              << static_cast<double>(finish - start) / CLOCKS_PER_SEC
              << std::endl;
    start = clock();
    for (uint i = 0; i < 30; ++i)
        lemiremaxmin C(data, width);
    finish = clock();
    std::cout << "lemire = "
              << static_cast<double>(finish - start) / CLOCKS_PER_SEC
              << std::endl;
    std::cout << "------------" << std::endl;
}

/**
 * use in conjunction with

 | awk '{print $2}'
 */
int main(int params, char ** args) {
    int whitesize = 0;
    int walksize = 0;
    int times = 1;
    int sinesize = 0;
    floattype sineperiod = 0.0;
    int windowbegin = 10;
    int windowend = 11;
    bool doslow = true;
    bool cininput = false;
    std::vector<floattype> data(0);
    for (int i = 1; i < params; ++i) {
        if (strcmp(args[i], "--skipslow") == 0) {
            doslow = false;
        }
        if (strcmp(args[i], "--pipedata") == 0) {
            cininput = true;
        }
        if (strcmp(args[i], "--unit") == 0) {
            std::cout << "OK " << std::endl;
            return 0;
        }
        if (strcmp(args[i], "--window") == 0) {
            if (params - i > 1) {
                windowbegin = atoi(args[++i]);
                windowend = windowbegin + 1;
            } else {
                std::cerr << "--window expects an integer (length)"
                          << std::endl;
                return -1;
            }
            continue;
        }
        if (strcmp(args[i], "--windowrange") == 0) {
            if (params - i > 2) {
                windowbegin = atoi(args[++i]);
                windowend = atoi(args[++i]) + 1;
            } else {
                std::cerr << "--windowrange expects two integers (length)"
                          << std::endl;
                return -1;
            }
            continue;
        }
        if (strcmp(args[i], "--white") == 0) {
            if (params - i > 1)
                whitesize = atoi(args[++i]);
            else {
                std::cerr << "--white expects an integer (length)" << std::endl;
                return -1;
            }
            continue;
        }
        if (strcmp(args[i], "--walk") == 0) {
            if (params - i > 1)
                walksize = atoi(args[++i]);
            else {
                std::cerr << "--walk expects an integer (length)" << std::endl;
                return -1;
            }
            continue;
        }
        if (strcmp(args[i], "--sine") == 0) {
            if (params - i > 1)
                sinesize = atoi(args[++i]);
            else {
                std::cerr << "--sine expects an integer (length)" << std::endl;
                return -1;
            }
            if (params - i > 1)
                sineperiod = atof(args[++i]);
            else {
                std::cerr << "--sine expects an integer (length) followed by a "
                             "float (freq) "
                          << std::endl;
                return -1;
            }
            continue;
        }
        if (strcmp(args[i], "--times") == 0) {
            if (params - i > 1)
                times = atoi(args[++i]);
            else {
                std::cerr << "--times expects an integer" << std::endl;
                return -1;
            }
            continue;
        }
    }
    std::cout << "# we report timings (in seconds) so lower is better" << std::endl;
    for (int window = windowbegin; window < windowend; ++window) {
        if (whitesize > 0) {
            std::cout << "# window = " << window << " whitesize = " << whitesize
                      << " times = " << times << " doslow = " << doslow
                      << std::endl;
            assert(window + 1 < whitesize);
            timings(window, whitesize, times, doslow);
        } else if (walksize > 0) {
            std::cout << "# window = " << window << " walksize = " << walksize
                      << " times = " << times << " doslow = " << doslow
                      << std::endl;
            walktimings(window, walksize, times, doslow);

        } else if (sinesize > 0) {
            std::cout << "# window = " << window << " sinesize = " << sinesize
                      << " sine period " << sineperiod << " times = " << times
                      << " doslow = " << doslow << std::endl;
            assert(window + 1 < sinesize);
            sinetimings(window, sinesize, sineperiod, times, doslow);
        } else {
            if ((data.empty()) && cininput) {
                data = getcin();
                std::cout << "# window = " << window << " times = " << times
                          << " doslow = " << doslow << std::endl;
                process(data, window, times, doslow);
            } else {
                std::cout << "Generating sine waves." << std::endl;
                for (uint width = 3; width <= 100; ++width) {
                    std::cout << "Using a min/max width of " << width
                              << std::endl;
                    sinetimings(width);
                }
            }
        }
    }
    return 0;
}
