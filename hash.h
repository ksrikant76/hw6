#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here

        //initialize w to have all 0s
        unsigned long long w[5] = {0, 0, 0, 0, 0};

        int len = static_cast<int>(k.size());
        int pos = len - 1; //last index
        int block = 4;  // start at last (4th) block of 6 chars

        //go through string right to left, processing in groups of 6
        while (pos >= 0 && block >= 0) {
            unsigned long long value = 0;
            unsigned long long weight = 1; // start with 36^0

            for (int i = 0; i < 6 && pos >= 0; ++i) {
                char c = k[pos];
                --pos; //move left
                HASH_INDEX_T digit = letterDigitToNumber(c);
                value += static_cast<unsigned long long>(digit) * weight;
                weight *= 36ULL; //next power of 36
            }

            w[block] = value;
            --block;
        }

        //combine w values and r values
        unsigned long long result = 0;
        for (int i = 0; i < 5; ++i) {
            result += static_cast<unsigned long long>(rValues[i]) * w[i];
        }
        return static_cast<HASH_INDEX_T>(result);

    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        char c = std::tolower(letter);
        if (c >= 'a' && c <= 'z') { //0-25
            return static_cast<HASH_INDEX_T>(c - 'a');
        }
        else if (c >= '0' && c <= '9') { //26-35
            return static_cast<HASH_INDEX_T>((c - '0') + 26);
        }
        return 0;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
