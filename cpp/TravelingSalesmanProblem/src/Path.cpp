#include "Path.h"
#include <iostream>
#include <vector>


unsigned Path::_assignId = 0;	/**< static,����ID */


/** \brief ����ID,static
 *
 * \return int
 *
 */
unsigned Path::assignId() { return ++_assignId; }


/** \brief ���캯��
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


/** \brief �������캯��
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


/** \brief ��ֵ
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


/** \brief �ȽϺ���������"=="
 *
 * \param rhs_ const Path&
 * \return bool
 *
 */
bool Path::operator==(const Path& rhs_) {
	return _pathCity == rhs_._pathCity;
}


/** \brief ��ӡ·���������Ϣ
 *
 * \return void
 *
 */
void  Path::printPath() const {
	std::cout << "��0��";
	for (const int &city: _pathCity) {
		std::cout << "��" << city << "��";
	}
	std::cout << "��0��";
	std::cout << "->[id=" << _id << ",d=" << _pathDistance
			<< ",f=" << _fitness << "]" << std::endl;
}
