/**
 * Copyright 2006-..., Daniel Lemire
 * Streaming Maximum-Minimum Filter Using No More than 3 Comparisons per Element
 *
 *  This program is free software; you can
 *  redistribute it and/or modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation (version 2). This
 *  program is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 *  details. You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef RUNNINGMAXMIN_H
#define RUNNINGMAXMIN_H

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <vector>

#include "common.h"
#include "deque.h"

inline void display(const std::vector<floattype> & a) {
    for (floattype i : a)
        std::cout << i << " ";
    std::cout << std::endl;
}

class minmaxfilter {
public:
    virtual std::vector<floattype> & getmaxvalues() = 0;
    virtual std::vector<floattype> & getminvalues() = 0;

    virtual ~minmaxfilter() = default;
};

/**
 * This is the naive algorithm one might try first.
 */
class slowmaxmin : public minmaxfilter {
public:
    slowmaxmin(std::vector<floattype> & array, int width)
        : maxvalues(array.size() - width + 1),
          minvalues(array.size() - width + 1) {
        floattype maxvalue, minvalue;
        for (uint s = 0; s < array.size() - width + 1; ++s) {
            maxvalue = array[s];
            minvalue = array[s];
            // could be done with iterators
            for (uint k = s + 1; k < s + width; ++k) {
                if (maxvalue < array[k])
                    maxvalue = array[k];
                if (minvalue > array[k])
                    minvalue = array[k];
            }
            maxvalues[s] = maxvalue;
            minvalues[s] = minvalue;
        }
    }
    std::vector<floattype> & getmaxvalues() {
        return maxvalues;
    }
    std::vector<floattype> & getminvalues() {
        return minvalues;
    }
    std::vector<floattype> maxvalues;
    std::vector<floattype> minvalues;
};

/**
 * This is an implementation of the patented Gil-Kimmel algorithm.
 * Could be rewritten to use less memory.
 */
class GilKimmel : public minmaxfilter {
public:
    GilKimmel(std::vector<floattype> & array, int width)
        : maxvalues(array.size() - width + 1),
          minvalues(array.size() - width + 1) {
        std::vector<floattype> R(array.size() + 1);
        std::vector<floattype> S(array.size() + 1);
        computePrefixSuffixMax(R, S, array,
                               width); // implements the cut in the middle trick
        for (int j = 0; j < static_cast<int>(array.size()) - width + 1;
             j += width) {
            const int endofblock =
                std::min(j + width, static_cast<int>(array.size()) - width + 1);
            int begin = j;
            int end = endofblock;
            int midpoint = (end - begin + 1) / 2 + begin;
            while (midpoint != end) {
                if (S[midpoint + width - 1] <= R[midpoint]) {
                    begin = midpoint;
                    midpoint = (end - begin + 1) / 2 + begin;
                } else {
                    end = midpoint;
                    midpoint = (end - begin + 1) / 2 + begin;
                }
            }
            for (int jj = j; jj < midpoint; ++jj) {
                maxvalues[jj] = R[jj];
            }
            for (int jj = midpoint; jj < endofblock; ++jj) {
                maxvalues[jj] = S[jj + width - 1];
            }
        }
        computePrefixSuffixMin(R, S, array,
                               width); // implements the cut in the middle trick
        for (int j = 0; j < static_cast<int>(array.size()) - width + 1;
             j += width) {
            const int endofblock =
                std::min(j + width, static_cast<int>(array.size()) - width + 1);
            int begin = j;
            int end = endofblock;
            int midpoint = (end - begin + 1) / 2 + begin;
            while (midpoint != end) {
                if (S[midpoint + width - 1] >= R[midpoint]) {
                    begin = midpoint;
                    midpoint = (end - begin + 1) / 2 + begin;
                } else {
                    end = midpoint;
                    midpoint = (end - begin + 1) / 2 + begin;
                }
            }
            for (int jj = j; jj < midpoint; ++jj) {
                minvalues[jj] = R[jj];
            }
            for (int jj = midpoint; jj < endofblock; ++jj) {
                minvalues[jj] = S[jj + width - 1];
            }
        }
    }
    void computePrefixSuffixMax(std::vector<floattype> & R,
                                std::vector<floattype> & S,
                                const std::vector<floattype> & array,
                                const int width) {
        for (int j = 0; j < static_cast<int>(array.size()); j += width) {
            const int begin = j;
            const int end = std::min(static_cast<int>(array.size()), j + width);
            const int midpoint = (end - begin + 1) / 2 + begin;
            S[begin] = array[begin];
            for (int jj = begin + 1; jj < midpoint; ++jj) {
                S[jj] = std::max(array[jj], S[jj - 1]);
            }
            R[end - 1] = array[end - 1];
            for (int jj = end - 2; jj >= midpoint; --jj) {
                R[jj] = std::max(R[jj + 1], array[jj]);
            }
            if (std::max(R[midpoint], S[midpoint - 1]) == R[midpoint]) {
                for (int jj = midpoint; jj < end; ++jj)
                    S[jj] = std::max(array[jj], S[jj - 1]);
                for (int jj = midpoint - 1; jj >= begin; --jj)
                    R[jj] = R[midpoint];
            } else {
                for (int jj = midpoint - 1; jj >= begin; --jj)
                    R[jj] = std::max(R[jj + 1], array[jj]);
                for (int jj = midpoint; jj < end; ++jj)
                    S[jj] = S[midpoint - 1];
            }
        }
    }

    void computePrefixSuffixMin(std::vector<floattype> & R,
                                std::vector<floattype> & S,
                                const std::vector<floattype> & array,
                                const int width) {
        for (int j = 0; j < static_cast<int>(array.size()); j += width) {
            const int begin = j;
            const int end = std::min(static_cast<int>(array.size()), j + width);
            const int midpoint = (end - begin + 1) / 2 + begin;
            S[begin] = array[begin];
            for (int jj = begin + 1; jj < midpoint; ++jj) {
                S[jj] = std::min(array[jj], S[jj - 1]);
            }
            R[end - 1] = array[end - 1];
            for (int jj = end - 2; jj >= midpoint; --jj) {
                R[jj] = std::min(R[jj + 1], array[jj]);
            }
            if (std::min(R[midpoint], S[midpoint - 1]) == R[midpoint]) {
                for (int jj = midpoint; jj < end; ++jj)
                    S[jj] = std::min(array[jj], S[jj - 1]);
                for (int jj = midpoint - 1; jj >= begin; --jj)
                    R[jj] = R[midpoint];
            } else {
                for (int jj = midpoint - 1; jj >= begin; --jj)
                    R[jj] = std::min(R[jj + 1], array[jj]);
                for (int jj = midpoint; jj < end; ++jj)
                    S[jj] = S[midpoint - 1];
            }
        }
    }
    std::vector<floattype> & getmaxvalues() {
        return maxvalues;
    }
    std::vector<floattype> & getminvalues() {
        return minvalues;
    }
    std::vector<floattype> maxvalues;
    std::vector<floattype> minvalues;
};

/**
 * This should be very close to the van Herk algorithm.
 */
class vanHerkGilWermanmaxmin : public minmaxfilter {
public:
    vanHerkGilWermanmaxmin(std::vector<floattype> & array, int width)
        : maxvalues(array.size() - width + 1),
          minvalues(array.size() - width + 1) {
        std::vector<floattype> R(width);
        std::vector<floattype> S(width);
        for (uint j = 0; j < array.size() - width + 1; j += width) {
            uint Rpos = std::min(j + width - 1,
                                 static_cast<unsigned>(array.size() - 1));
            R[0] = array[Rpos];
            for (uint i = Rpos - 1; i + 1 > j; i -= 1)
                R[Rpos - i] = std::max(R[Rpos - i - 1], array[i]);
            S[0] = array[Rpos];
            uint m1 = std::min(j + 2 * width - 1,
                               static_cast<unsigned>(array.size()));
            for (uint i = Rpos + 1; i < m1; ++i) {
                S[i - Rpos] = std::max(S[i - Rpos - 1], array[i]);
            }
            for (uint i = 0; i < m1 - Rpos; i += 1)
                maxvalues[j + i] = std::max(S[i], R[(Rpos - j + 1) - i - 1]);
        }
        for (uint j = 0; j < array.size() - width + 1; j += width) {
            uint Rpos = std::min(j + width - 1,
                                 static_cast<unsigned>(array.size() - 1));
            R[0] = array[Rpos];
            for (uint i = Rpos - 1; i + 1 > j; i -= 1)
                R[Rpos - i] = std::min(R[Rpos - i - 1], array[i]);
            S[0] = array[Rpos];
            uint m1 = std::min(j + 2 * width - 1,
                               static_cast<unsigned>(array.size()));
            for (uint i = Rpos + 1; i < m1; ++i) {
                S[i - Rpos] = std::min(S[i - Rpos - 1], array[i]);
            }
            for (uint i = 0; i < m1 - Rpos; i += 1)
                minvalues[j + i] = std::min(S[i], R[(Rpos - j + 1) - i - 1]);
        }
        assert(maxvalues.size() == array.size() - width + 1);
        assert(minvalues.size() == array.size() - width + 1);
    }
    std::vector<floattype> & getmaxvalues() {
        return maxvalues;
    }
    std::vector<floattype> & getminvalues() {
        return minvalues;
    }
    std::vector<floattype> maxvalues;
    std::vector<floattype> minvalues;
};

/**
 * implementation of the streaming algorithm
 */
class lemiremaxmin : public minmaxfilter {
public:
    lemiremaxmin(std::vector<floattype> & array, uint width)
        : maxvalues(array.size() - width + 1),
          minvalues(array.size() - width + 1) {
        std::deque<int> maxfifo, minfifo;
        for (uint i = 1; i < width; ++i) {
            if (array[i] > array[i - 1]) { // overshoot
                minfifo.push_back(i - 1);
                while (!maxfifo.empty()) {
                    if (array[i] <= array[maxfifo.back()]) {
                        if (i == width + maxfifo.front())
                            maxfifo.pop_front();
                        break;
                    }
                    maxfifo.pop_back();
                }
            } else {
                maxfifo.push_back(i - 1);
                while (!minfifo.empty()) {
                    if (array[i] >= array[minfifo.back()]) {
                        if (i == width + minfifo.front())
                            minfifo.pop_front();
                        break;
                    }
                    minfifo.pop_back();
                }
            }
        }
        for (uint i = width; i < array.size(); ++i) {
            maxvalues[i - width] =
                array[maxfifo.empty() ? i - 1 : maxfifo.front()];
            minvalues[i - width] =
                array[minfifo.empty() ? i - 1 : minfifo.front()];
            if (array[i] > array[i - 1]) { // overshoot
                minfifo.push_back(i - 1);
                if (i == width + minfifo.front())
                    minfifo.pop_front();
                while (!maxfifo.empty()) {
                    if (array[i] <= array[maxfifo.back()]) {
                        if (i == width + maxfifo.front())
                            maxfifo.pop_front();
                        break;
                    }
                    maxfifo.pop_back();
                }
            } else {
                maxfifo.push_back(i - 1);
                if (i == width + maxfifo.front())
                    maxfifo.pop_front();
                while (!minfifo.empty()) {
                    if (array[i] >= array[minfifo.back()]) {
                        if (i == width + minfifo.front())
                            minfifo.pop_front();
                        break;
                    }
                    minfifo.pop_back();
                }
            }
        }
        maxvalues[array.size() - width] =
            array[maxfifo.empty() ? array.size() - 1 : maxfifo.front()];
        minvalues[array.size() - width] =
            array[minfifo.empty() ? array.size() - 1 : minfifo.front()];
    }
    std::vector<floattype> & getmaxvalues() {
        return maxvalues;
    }
    std::vector<floattype> & getminvalues() {
        return minvalues;
    }
    std::vector<floattype> maxvalues;
    std::vector<floattype> minvalues;
};

// actual streaming implementation
class lemiremaxmintruestreaming {
public:
    explicit lemiremaxmintruestreaming(uint width)
        : up(), lo(), n(0), ww(width) {
        init(&up, ww);
        init(&lo, ww);
    }

    ~lemiremaxmintruestreaming() {
        free(&up);
        free(&lo);
    }

    void update(floattype value) {
        if (nonempty(&up) != 0) {
            if (value > tailvalue(&up)) {
                prunetail(&up);
                while (((nonempty(&up)) != 0) && (value >= tailvalue(&up))) {
                    prunetail(&up);
                }
            } else {
                prunetail(&lo);
                while (((nonempty(&lo)) != 0) && (value <= tailvalue(&lo))) {
                    prunetail(&lo);
                }
            }
        }
        push(&up, n, value);
        if (n == ww + headindex(&up)) {
            prunehead(&up);
        }

        push(&lo, n, value);
        if (n == ww + headindex(&lo)) {
            prunehead(&lo);
        }
        n++;
    }

    floattype max() {
        return headvalue(&up);
    }
    floattype min() {
        return headvalue(&lo);
    }

    intfloatqueue up;
    intfloatqueue lo;
    uint n;
    uint ww;
};

// wrapper over the streaming version
class lemiremaxminwrap : public minmaxfilter {
public:
    lemiremaxminwrap(std::vector<floattype> & array, uint width)
        : maxvalues(array.size() - width + 1),
          minvalues(array.size() - width + 1) {
        lemiremaxmintruestreaming lts(width);
        for (uint i = 0; i < width - 1; ++i) {
            lts.update(array[i]);
        }
        for (uint i = width - 1; i < array.size(); ++i) {
            lts.update(array[i]);
            maxvalues[i - width + 1] = lts.max();
            minvalues[i - width + 1] = lts.min();
        }
    }
    std::vector<floattype> & getmaxvalues() {
        return maxvalues;
    }
    std::vector<floattype> & getminvalues() {
        return minvalues;
    }
    std::vector<floattype> maxvalues;
    std::vector<floattype> minvalues;
};

#include "mono_wedge.h"

struct Sample {
	floattype value;
	uint   time;

	bool operator<(const Sample &o) const    {return value < o.value;}
	bool operator>(const Sample &o) const    {return value > o.value;}
};
// wrapper over the monowedge streaming version
class monowedgewrap : public minmaxfilter {
public:
    monowedgewrap(std::vector<floattype> & array, uint width)
        : maxvalues(array.size() - width + 1),
          minvalues(array.size() - width + 1) {
        std::deque<Sample> max_wedge;
        std::deque<Sample> min_wedge;
        for (uint i = 0; i < width - 1; ++i) {
            Sample sample = {array[i], i};
            mono_wedge::max_wedge_update(max_wedge, sample);
            mono_wedge::min_wedge_update(min_wedge, sample);
        }
        for (uint i = width; i < array.size(); ++i) {
            Sample sample = {array[i], i};
            mono_wedge::max_wedge_update(max_wedge, sample);
            mono_wedge::min_wedge_update(min_wedge, sample);
            while (max_wedge.front().time <= i-width) max_wedge.pop_front();
            while (min_wedge.front().time <= i-width) min_wedge.pop_front();
            maxvalues[i - width + 1] = max_wedge.front().value;
            minvalues[i - width + 1] = min_wedge.front().value;
        }
    }
    std::vector<floattype> & getmaxvalues() {
        return maxvalues;
    }
    std::vector<floattype> & getminvalues() {
        return minvalues;
    }
    std::vector<floattype> maxvalues;
    std::vector<floattype> minvalues;
};


/**
 * implementation of the bitmap-based streaming algorithm
 */
class lemirebitmapmaxmin : public minmaxfilter {
public:
    // TODO: make the code portable to non-GCC-like compilers
    // TODO: extend beyond 64-bit to include 128-bit
    lemirebitmapmaxmin(std::vector<floattype> & array, const uint width)
        : maxvalues(array.size() - width + 1),
          minvalues(array.size() - width + 1) {
        assert(width <= sizeof(unsigned long) * 8);
        unsigned long maxfifo = 0;
        unsigned long minfifo = 0;
        for (uint i = 1; i < width; ++i) {
            if (array[i] > array[i - 1]) { // overshoot
                minfifo |= 1;
                minfifo <<= 1;
                maxfifo <<= 1;
                while (maxfifo != 0) {
                    const long t = maxfifo & -maxfifo;
                    const int bitpos = __builtin_popcountl(t - 1);
                    if (array[i] <= array[i - bitpos]) {
                        break;
                    }
                    maxfifo ^= t;
                }
            } else {
                maxfifo |= 1;
                minfifo <<= 1;
                maxfifo <<= 1;
                while (minfifo != 0) {
                    const long t = minfifo & -minfifo;
                    const int bitpos = __builtin_popcountl(t - 1);
                    if (array[i] >= array[i - bitpos]) {
                        break;
                    }
                    minfifo ^= t;
                }
            }
        }
        unsigned long mask = ~0l;
        if (width < sizeof(unsigned long) * 8) {
            mask = (1UL << width) - 1;
        }
        for (uint i = width; i < array.size(); ++i) {
            maxfifo &= mask;
            minfifo &= mask;
            if (maxfifo == 0)
                maxvalues[i - width] = array[i - 1];
            else {
                maxvalues[i - width] = array[i - (sizeof(unsigned long) * 8 -
                                                  __builtin_clzl(maxfifo))];
            }
            if (minfifo == 0)
                minvalues[i - width] = array[i - 1];
            else {
                minvalues[i - width] = array[i - (sizeof(unsigned long) * 8 -
                                                  __builtin_clzl(minfifo))];
            }
            if (array[i] > array[i - 1]) { // overshoot
                minfifo |= 1;
                minfifo <<= 1;
                maxfifo <<= 1;
                while (maxfifo != 0) {
                    const long t = maxfifo & -maxfifo;
                    const int bitpos = __builtin_popcountl(t - 1);
                    if (array[i] <= array[i - bitpos]) {
                        break;
                    }
                    maxfifo ^= t;
                }
            } else {
                maxfifo |= 1;
                maxfifo <<= 1;
                minfifo <<= 1;
                while (minfifo != 0) {
                    const long t = minfifo & -minfifo;
                    const int bitpos = __builtin_popcountl(t - 1);
                    if (array[i] >= array[i - bitpos]) {
                        break;
                    }
                    minfifo ^= t;
                }
            }
        }
        maxfifo = maxfifo & mask;
        minfifo = minfifo & mask;
        if (maxfifo == 0)
            maxvalues[array.size() - width] = array[array.size() - 1];
        else
            maxvalues[array.size() - width] =
                array[array.size() -
                      (sizeof(unsigned long) * 8 - __builtin_clzl(maxfifo))];
        if (minfifo == 0)
            minvalues[array.size() - width] = array[array.size() - 1];
        else
            minvalues[array.size() - width] =
                array[array.size() -
                      (sizeof(unsigned long) * 8 - __builtin_clzl(minfifo))];
    }
    std::vector<floattype> & getmaxvalues() {
        return maxvalues;
    }
    std::vector<floattype> & getminvalues() {
        return minvalues;
    }
    std::vector<floattype> maxvalues;
    std::vector<floattype> minvalues;
};

/**
 * simplest implementation (pseudocode-like)
 */
class simplelemiremaxmin : public minmaxfilter {
public:
    simplelemiremaxmin(std::vector<floattype> & array, uint width)
        : maxvalues(array.size() - width + 1),
          minvalues(array.size() - width + 1) {
        std::deque<int> maxfifo, minfifo;
        maxfifo.push_back(0);
        minfifo.push_back(0);
        for (uint i = 1; i < width; ++i) {
            if (array[i] > array[i - 1]) { // overshoot
                maxfifo.pop_back();
                while (!maxfifo.empty()) {
                    if (array[i] <= array[maxfifo.back()])
                        break;
                    maxfifo.pop_back();
                }
            } else {
                minfifo.pop_back();
                while (!minfifo.empty()) {
                    if (array[i] >= array[minfifo.back()])
                        break;
                    minfifo.pop_back();
                }
            }
            maxfifo.push_back(i);
            minfifo.push_back(i);
        }
        for (uint i = width; i < array.size(); ++i) {
            maxvalues[i - width] = array[maxfifo.front()];
            minvalues[i - width] = array[minfifo.front()];
            if (array[i] > array[i - 1]) { // overshoot
                maxfifo.pop_back();
                while (!maxfifo.empty()) {
                    if (array[i] <= array[maxfifo.back()])
                        break;
                    maxfifo.pop_back();
                }
            } else {
                minfifo.pop_back();
                while (!minfifo.empty()) {
                    if (array[i] >= array[minfifo.back()])
                        break;
                    minfifo.pop_back();
                }
            }
            maxfifo.push_back(i);
            minfifo.push_back(i);
            if (i == width + maxfifo.front())
                maxfifo.pop_front();
            else if (i == width + minfifo.front())
                minfifo.pop_front();
        }
        maxvalues[array.size() - width] = array[maxfifo.front()];
        minvalues[array.size() - width] = array[minfifo.front()];
    }
    std::vector<floattype> & getmaxvalues() {
        return maxvalues;
    }
    std::vector<floattype> & getminvalues() {
        return minvalues;
    }
    std::vector<floattype> maxvalues;
    std::vector<floattype> minvalues;
};

#endif
