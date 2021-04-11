#pragma once

#include <string>
#include "../../common/List/List.h"
#include "../../common/FileHelper/FileHelper.h"

typedef unsigned long long BigInt;

using namespace std;
using namespace files;

namespace algorithms {
	class Shamir—ipher {
	private:

		int p;

		long extended_euclid(long a, long b, long* x, long* y);
		long inverse(long a, long n);

		void getInversePiar(long *first, long *second, int left, int right);

		BigInt gcd(BigInt a, BigInt b);
		BigInt mul(BigInt a, BigInt b, BigInt m);
		BigInt pows(BigInt a, BigInt b, BigInt m);

		int random(int left, int right);

		List<BigInt> transformation(const List<BigInt> previous, long power);

	public:
		Shamir—ipher(int p);
		
		bool isPrimeNumber(long long x);
		
		void run(string text, FileHelper helper);
	};
}