#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>

/** \brief Ëæ»úÊýÀà
 */
class Random {

public:

	Random() = delete;

	static int nextInt(int low, int high);

	static double nextDouble(double low = 0.0d, double high = 1.0d);

};

#endif // RANDOM_H
