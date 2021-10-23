#ifndef __PATH_H__
#define __PATH_H__

#include <vector>
using std::vector;


/** \brief ·����
 */
class Path {

public:

/**< ���츳ֵ���� */

	Path() = default;

	Path(const vector<int> &path_);

	Path(const Path &other_);

	Path& operator=(const Path &other_);

	bool operator==(const Path &rhs_);

	/// ɾ��
	vector<int>& citys() { return _pathCity; }

/**< setter �� getter ���� */

	int getId() const { return _id; }

	double getFitness() const { return _fitness; }

	void setFitness(double fitness_) { _fitness = fitness_; }

	double getPathDistance() const { return _pathDistance; }

	void setPathDistance(double pathDistance_) { _pathDistance = pathDistance_; }

	int getCity(unsigned no_) const { return _pathCity[no_]; }

	void setCity(unsigned no_, int city) { _pathCity[no_] = city; }

	void printPath() const;

	void setId(unsigned id_) { _id = id_; }

private:

/**< ˽�к��� */

	static unsigned assignId();

/**< ˽�г�Ա���� */

	static unsigned _assignId;			/**< static,����ID */

	unsigned _id;						/**< ·��ID */

	double _fitness;					/**< ��Ӧ�� */

	double _pathDistance;				/**< ·���� */

	vector<int> _pathCity;				/**< ·���ĳ��� */

};

#endif // __PATH_H__
