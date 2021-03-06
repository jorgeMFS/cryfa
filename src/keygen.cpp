/**
 * @file      keygen.cpp
 * @brief     Key generator
 * @author    Morteza Hosseini  (seyedmorteza@ua.pt)
 * @author    Diogo Pratas      (pratas@ua.pt)
 * @author    Armando J. Pinho  (ap@ua.pt)
 * @copyright The GNU General Public License v3.0
 */

#include <iostream>
#include "def.hpp"
#include "fn.hpp"
#include "assert.hpp"
using std::cerr;
using std::cin;
using std::string;
using std::uniform_int_distribution;
using std::ofstream;

int main (int argc, char* argv[]) {
  try {
    constexpr u32 keyLen     = 1024;
    constexpr byte nShowable = 94;  // No. showable ascii chars
    constexpr byte firstC    = 33;  // First showable char: '!'
  
    cerr << "Enter a password, then press 'Enter':\n";
    string pass;
    for (char c; cin.get(c) && c != '\n';)
      pass += c;
  
    cerr << "Enter a file name to save the generated key, "
            "then press 'Enter':\n";
    string target;
    for (char c; cin.get(c) && c != '\n';) {
      assert(c == ' ', "Error: the file name has a space character.\n");
      target += c;
    }
    
    uniform_int_distribution< rng_t::result_type > udist(0, 255);
    rng_t rng;
    // Using old random to generate the new random initSeed
    srandom(static_cast<u32>(36721 *
            (94583 * accum_even(pass.begin(), pass.end(), 0ul) +
            279431 * accum_odd(pass.begin(), pass.end(), 0ul)) + 623681));
    u64 initSeed = 0;
    for (char c : pass)
      initSeed += c * random() + random();
    rng.seed(static_cast<rng_t::result_type>(initSeed));
  
    string key;
    for (auto i = keyLen; i--;)
      key += static_cast<char>(
          ((udist(rng) * accum_hops(pass.begin(), pass.end(), 0u, i+1))
           % nShowable) + firstC);
  
    ofstream keyFile(target);
    keyFile << key;
  }
  catch (std::exception& e) { cerr << e.what(); }
  catch (...) { return EXIT_FAILURE; }
}