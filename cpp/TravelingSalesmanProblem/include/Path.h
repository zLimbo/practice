#ifndef __PATH_H__
#define __PATH_H__

#include <vector>
using std::vector;


/** \brief 路径类
 */
class Path {

public:

/**< 构造赋值函数 */

	Path() = default;

	Path(const vector<int> &path_);

	Path(const Path &other_);

	Path& operator=(const Path &other_);

	bool operator==(const Path &rhs_);

	/// 删除
	vector<int>& citys() { return _pathCity; }

/**< setter 和 getter 函数 */

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

/**< 私有函数 */

	static unsigned assignId();

/**< 私有成员变量 */

	static unsigned _assignId;			/**< static,分配ID */

	unsigned _id;						/**< 路径ID */

	double _fitness;					/**< 适应度 */

	double _pathDistance;				/**< 路径长 */

	vector<int> _pathCity;				/**< 路径的城市 */

};

#endif // __PATH_H__
