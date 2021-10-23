#include "Random.h"



/** \brief 随机整数
 *
 * \param low int
 * \param high int
 * \return int
 *
 */
int Random::nextInt(int low, int high) {
	return rand() % (high - low) + low;
}


/** \brief 随机实数
 *
 * \param low=0.0d double
 * \param high=1.0d double
 * \return double
 *
 */
double Random::nextDouble(double low/*=0.0d*/, double high/*=1.0d*/) {
	return double(rand()) / double(RAND_MAX) * (high - low) + low;
}
