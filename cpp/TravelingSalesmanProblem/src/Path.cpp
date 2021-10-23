#include "Path.h"
#include <iostream>
#include <vector>


unsigned Path::_assignId = 0;	/**< static,分配ID */


/** \brief 分配ID,static
 *
 * \return int
 *
 */
unsigned Path::assignId() { return ++_assignId; }


/** \brief 构造函数
 *
 * \param pathCity_ const std::vector<int>&
 *
 */
Path::Path(const std::vector<int> &pathCity_):
	_id(assignId()),
	_fitness(0),
	_pathDistance(0),
	_pathCity(pathCity_)
{
	//
}


/** \brief 拷贝构造函数
 *
 * \param other_ const Path&
 *
 */
Path::Path(const Path& other_):
	_id(assignId()),
	_fitness(other_._fitness),
	_pathDistance(other_._pathDistance),
	_pathCity(other_._pathCity)
{
	//
}


/** \brief 赋值
 *
 * \param rhs_ const Path&
 * \return Path&
 *
 */
Path& Path::operator=(const Path& rhs_) {
	if (this == &rhs_)
		return *this;
	_id = rhs_._id;
	_fitness = rhs_._fitness;
	_pathDistance = rhs_._pathDistance;
	_pathCity = rhs_._pathCity;
	return *this;
}


/** \brief 比较函数，重载"=="
 *
 * \param rhs_ const Path&
 * \return bool
 *
 */
bool Path::operator==(const Path& rhs_) {
	return _pathCity == rhs_._pathCity;
}


/** \brief 打印路径及相关信息
 *
 * \return void
 *
 */
void  Path::printPath() const {
	std::cout << "【0】";
	for (const int &city: _pathCity) {
		std::cout << "【" << city << "】";
	}
	std::cout << "【0】";
	std::cout << "->[id=" << _id << ",d=" << _pathDistance
			<< ",f=" << _fitness << "]" << std::endl;
}
