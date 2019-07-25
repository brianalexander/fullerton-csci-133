#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdint>
#include "./math/distributions/chi_squared.hpp"

#define m ((int) std::pow(2,16)) 
#define NUM_BINS 65536
#define DOF 65535
#define HIST_NUM_BINS 20
#define HIST_HEIGHT 70
#define MULTIPLICATIVE_CONSTANT (std::sqrt(3) - 1) / 2.0

// typedefs
typedef uint16_t (*takesSTRINGreturnsUINT16_T)(std::string*);
typedef std::vector<uint16_t> HashVector;

// Hashing Function Prototypes
uint16_t StringLengthHash(std::string* word);
uint16_t FirstCharacterHash(std::string* word);
uint16_t AdditiveChecksumHash(std::string* word);
uint16_t RemainderHash(std::string* word);
uint16_t MultiplicativeHash(std::string* word);

// Utility Functions
void DispHashValues(HashVector* hashes);
int ApplyHashFunction(
          std::ifstream* wordList, 
          HashVector* hashes, 
          takesSTRINGreturnsUINT16_T fptr
    );
float calculateChiSquared(HashVector* hashes, int totalItemsHashed);


// ==================================================
// main
// ==================================================
int main(void) {
  std::ifstream wordList;
  HashVector hashes(65536, 0); // reserve 65,536 slots; initialize to zeros
  int numberOfHashedItems;

  // open wordList for reading
  wordList.open("words.txt");

  // String Length
  numberOfHashedItems = ApplyHashFunction(&wordList, &hashes, StringLengthHash);
  std::cout << "String-Length Chi-Squared: " << 
              calculateChiSquared(&hashes, numberOfHashedItems) << std::endl;
  DispHashValues(&hashes);
  // First Character
  numberOfHashedItems = ApplyHashFunction(&wordList, &hashes, FirstCharacterHash);
  std::cout << std::endl;
  std::cout << "First-Character Chi-Squared: " << 
              calculateChiSquared(&hashes, numberOfHashedItems) << std::endl;
  DispHashValues(&hashes);
  // Additive Checksum
  numberOfHashedItems = ApplyHashFunction(&wordList, &hashes, AdditiveChecksumHash);
  std::cout << std::endl;
  std::cout << "Additive Chi-Squared: " << 
              calculateChiSquared(&hashes, numberOfHashedItems) << std::endl;
  DispHashValues(&hashes);
  // Remainder 
  numberOfHashedItems = ApplyHashFunction(&wordList, &hashes, RemainderHash);
  std::cout << std::endl;
  std::cout << "Remainder Chi-Squared: " << 
              calculateChiSquared(&hashes, numberOfHashedItems) << std::endl;
  DispHashValues(&hashes);
  // Multiplicative
  numberOfHashedItems = ApplyHashFunction(&wordList, &hashes, MultiplicativeHash);
  std::cout << std::endl;
  std::cout << "Multiplicative Chi-Squared: " << 
              calculateChiSquared(&hashes, numberOfHashedItems) << std::endl;
  DispHashValues(&hashes);

  // close wordList
  wordList.close();

}

// ==================================================
// Hash Functions
// ==================================================
uint16_t StringLengthHash(std::string* word) {
  return word->size() % m;
}

uint16_t FirstCharacterHash(std::string* word) {
  return (unsigned char) word->at(0);
}

uint16_t AdditiveChecksumHash(std::string* word) {
  int sum;
  for (unsigned char c : *word) {
    sum += c;
  }
  return sum % m;
}

uint16_t RemainderHash(std::string* word) {
  int k = 0;
  for(unsigned char c : *word) {
    if(c < 0)
      std::cout << "out of range: " << c << std::endl;

    k = (c) + (k * 256) % 65413;
  }

  return k;
}

uint16_t MultiplicativeHash(std::string* word) {
  double k = 0;
  double A = MULTIPLICATIVE_CONSTANT;
  for(unsigned char c : *word) {
    if(c < 0)
      std::cout << "out of range: " << c << std::endl;
    k = std::fmod((A * c) + (A * k * 256), 1);
  }
  
  return std::floor(k*m);
}

// ==================================================
// Utility Functions
// ==================================================
void DispHashValues(HashVector* hashes) {
  float binCount[HIST_NUM_BINS] = {};
  int itemsPerBin = hashes->size() / HIST_NUM_BINS;
  int binIndex = 0;

  // Sum items per bin(hash) per bin(hist)
  // step through each hash adding count to binCount array
  // increment binCount to next array item when (i+1) % itemsPerBin == 0
  for (int i = 0; i < hashes->size(); i++) {
    if ((i+1) % itemsPerBin == 0) {
      binIndex++;
    }
    binCount[binIndex] += hashes->at(i);
  }
  
  // find max element value
  int maxValue = 0;
  for(int i = 0; i < HIST_NUM_BINS; i++) {
    if (binCount[i] > maxValue) {
      maxValue = binCount[i];
    }
  }

  // divide each element by max value to get in range 0...1
  for(int i = 0; i < HIST_NUM_BINS; i++) {
    binCount[i] = (binCount[i] / maxValue) * HIST_HEIGHT;
  }

  for(int i = 0; i < HIST_NUM_BINS; i++) {
    for(int j = 0; j < binCount[i]; j++) {
      std::cout << '-';
    }
    std::cout << std::endl;
  }
}

int ApplyHashFunction(
          std::ifstream* wordList, 
          HashVector* hashes, 
          takesSTRINGreturnsUINT16_T fptr
) {
  // ensure all values are zero before beginning counts.
  std::fill(hashes->begin(), hashes->end(), 0);

  uint16_t hash;
  int count = 0;

  for(std::string word; std::getline(*wordList, word); ) {
    hash = fptr(&word);
    hashes->at(hash)++;
    count++;
  }

  wordList->clear(); // clear EOF flag
  wordList->seekg(0, std::ios::beg); // move cursor to beginning of file

  return count;
}

float calculateChiSquared(HashVector* hashes, int totalItemsHashed) {
  float c2 = 0;
  float expected = totalItemsHashed / (1.0 * NUM_BINS);
  boost::math::chi_squared c2d(DOF);

  for (uint16_t count : *hashes) {
    c2 += pow(expected - count, 2) / expected;
  }

  return boost::math::cdf(c2d, c2);
}
