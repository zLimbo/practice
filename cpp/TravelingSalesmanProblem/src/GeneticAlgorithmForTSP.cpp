#include "GeneticAlgorithmForTSP.h"
#include "Random.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stack>
#include <vector>

using namespace std;


/** \brief 构造函数
 *
 * \param fileName_ const string&
 *
 */
GeneticAlgorithmForTSP::GeneticAlgorithmForTSP(const string &fileName_) {
	ifstream fileInputStream(fileName_, ios_base::in);
	string line;
	int lineNo = 0;
	while (getline(fileInputStream, line)) {
		istringstream inputStringStream(line);
		double distance;
		_cityDistance.emplace_back(vector<double>());
		while (inputStringStream >> distance) {
			_cityDistance[lineNo].emplace_back(distance);
		}
		++lineNo;
	}
	_cityNum = lineNo;
	_pathLength = _cityNum - 1;
	_populationNum = 20;
	_isDisplay = false;
	_isFullPermutationDisplay = false;
	_displayNum = 1;
	_fitnessIndex = 1;
}


/** \brief 算法运行
 *
 * \return void
 *
 */
void GeneticAlgorithmForTSP::run() {
	initial();
	_currGenerationNo = 0;
	while (++_currGenerationNo <= _generationNum) {
		geneticPopulation();
		if (_isDisplay && _currGenerationNo % _displayNum == 0) {
			printPopulation();
		}
	}
}


/** \brief 初始化
 *
 * \return void
 *
 */
void GeneticAlgorithmForTSP::initial() {
	_pathPopulation.clear();
	vector<int> path;
	for (unsigned i = 0; i < _pathLength; ++i) {
		path.emplace_back(i+1);
	}

//	unsigned count = 0;
//	while (++count <= _populationNum) {
//		_pathPopulation.emplace_back(path);
//	}

	unsigned count = 0;

	while (count < _populationNum) {
		random_shuffle(path.begin(), path.end());
		if (find(_pathPopulation.begin(), _pathPopulation.end(), path)
				== _pathPopulation.end()) {
			_pathPopulation.emplace_back(path);
			++count;
		}
	}

	calculateFitness();
	_minDistancePath.setPathDistance(double(unsigned(-1)));
	_minCount = 0;
//	printPopulation();
}


/** \brief 计算适应度
 *
 * \return void
 *
 */
void GeneticAlgorithmForTSP::calculateFitness() {
	double totalReci = 0;
	vector<double> pathReci(_populationNum);

	for (unsigned pathNo = 0; pathNo < _populationNum; ++pathNo) {
		Path &path = _pathPopulation[pathNo];
		/// 计算路径
		double pathDistance = calculatePathDistance(path);
		/// 设置路径
		path.setPathDistance(pathDistance);
		/// 计算路径的倒数和倒数之和
		pathReci[pathNo] = fitnessFunc(1.0d / pathDistance );
		totalReci += pathReci[pathNo];
	}
	/// 根据倒数求适应度
	for (unsigned pathNo = 0; pathNo < _populationNum; ++pathNo) {
		_pathPopulation[pathNo].setFitness(pathReci[pathNo] / totalReci);
		/// 和最短路线比较，如果比其短，则更新为它
		updateMinDistPath(_pathPopulation[pathNo]);
	}
}


/** \brief 用路径倒数作为参数的适应度函数
 *
 * \param pathReci_ double
 * \return double
 *
 */
double GeneticAlgorithmForTSP::fitnessFunc(double pathReci_) const {
	 double retValue = pathReci_;
	 for (unsigned i = 1; i < _fitnessIndex; ++i) {
		retValue *= pathReci_;
	 }
	 return retValue;
}


/** \brief 计算某一路径的长度
 *
 * \param path const Path&
 * \return double
 *
 */
double GeneticAlgorithmForTSP::calculatePathDistance(const Path &path) const {
	/// 计算路径
	double pathDistance = _cityDistance[0][path.getCity(0)];
	for (size_t i = 0; i < _pathLength-1; ++i) {
		pathDistance += _cityDistance[path.getCity(i)][path.getCity(i+1)];
	}
	pathDistance += _cityDistance[path.getCity(_pathLength-1)][0];
	/// 返回
	return pathDistance;
}


/** \brief 打印当前种群（路径）
 *
 * \return void
 *
 */
void GeneticAlgorithmForTSP::printPopulation() const {
	cout << "------> 第 " << _currGenerationNo << " 代种群" << endl;
	for (const Path &path: _pathPopulation) {
		path.printPath();
	}
}


/** \brief 打印城市距离二维表
 *
 * \return void
 *
 */
void GeneticAlgorithmForTSP::printCityDistance() const {
	cout << "  ";
	for (unsigned city = 0; city < _cityNum; ++city) {
		cout << "【" << city << "】";
	}
	cout << endl;
	for (unsigned row = 0; row < _cityNum; ++row) {
		cout << "【" << row << "】";
		for (unsigned col = 0; col < _cityNum; ++col) {
			std::cout << fixed << setprecision(2) << _cityDistance[row][col] << " ";
		}
		std::cout << std::endl;
	}
}


/** \brief 后代遗传，产生新种群
 *
 * \return void
 *
 */
void GeneticAlgorithmForTSP::geneticPopulation() {
	vector<Path> newPathPopulation;
	unsigned count = 0;
	while (count < _populationNum) {
		if (Random::nextDouble() < 0.5d) {	/// 变异
			newPathPopulation.push_back(pathVariation(getPathByFitness()));
			++count;
		} else {	/// 交叉
			Path path1 = getPathByFitness();
			Path path2 = getPathByFitness();
			pathCross(path1, path2);
			newPathPopulation.push_back(path1);
			newPathPopulation.push_back(path2);
			count += 2;
		}
	}
	/// 新种群替代旧种群
	swap(newPathPopulation, _pathPopulation);
	calculateFitness();
}


/** \brief 由适应度随机选择种群中的某一路径
 *
 * \return Path&
 *
 */
Path& GeneticAlgorithmForTSP::getPathByFitness() {
	double dialFitness = Random::nextDouble(0.0d, 1.0d);
	for (size_t pathNo = 0; pathNo < _populationNum; ++pathNo) {
		Path &path = _pathPopulation[pathNo];
		if (dialFitness < path.getFitness()) {
			return path;
		}
		dialFitness -= path.getFitness();
	}
	return _pathPopulation.back();
}


/** \brief 交叉
 *
 * \param path1_ Path
 * \param path2_ Path
 * \return void
 *
 */
void GeneticAlgorithmForTSP::pathCross(Path &path1_, Path &path2_) {
	vector<int> &path1Citys = path1_.citys();
	vector<int> &path2Citys = path2_.citys();
	int cityBegNo = Random::nextInt(0, _pathLength-1);
	int cityEndNo = Random::nextInt(0, _pathLength+1);
	while (cityBegNo >= cityEndNo) {
		cityBegNo = Random::nextInt(0, _pathLength);
		cityEndNo = Random::nextInt(0, _pathLength+1);
	}
	int crossLength = cityEndNo - cityBegNo;	/// 交叉长度
	/// 获取基因片段
	vector<int> crossPiece1(path1Citys.begin()+cityBegNo, path1Citys.begin()+cityEndNo);
	vector<int> crossPiece2(path2Citys.begin()+cityBegNo, path2Citys.begin()+cityEndNo);

//	cout << "#######################################" << endl;
//
//	path1_.printPath();
//	path2_.printPath();
//
//	cout << "@ crossLength = " << crossLength << " " << crossPiece1.size() << " " << crossPiece2.size() << endl;
//	cout << "@ cross1 : ";
//	for (auto &val: crossPiece1) { cout << val << " "; }
//	cout << endl;
//	cout << "@ cross2 : ";
//	for (auto &val: crossPiece2) { cout << val << " "; }
//	cout << endl;

	for (int i = 0; i < crossLength; ++i) {		/// 去重
		auto iter1 = find(path1Citys.begin(), path1Citys.end(), crossPiece2[i]);
		auto iter2 = find(path2Citys.begin(), path2Citys.end(), crossPiece1[i]);
		if (iter1 != path1_.citys().end()) {
//			cout << "@ find at 1 : " << *iter1 << endl;
			path1Citys.erase(iter1);
		}
		if (iter2 != path2_.citys().end()) {
//			cout << "@ find at 2 : " << *iter2 << endl;
			path2Citys.erase(iter2);
		}
	}
	/// 插入
	path1Citys.insert(path1Citys.begin()+cityBegNo, crossPiece2.begin(), crossPiece2.end());
	path2Citys.insert(path2Citys.begin()+cityBegNo, crossPiece1.begin(), crossPiece1.end());
//
//	path1_.printPath();
//	path2_.printPath();
//	cout << "#######################################" << endl;
}


/** \brief 由当前路径变异产生子代
 *
 * \param path_ Path
 * \return Path
 *
 */
Path GeneticAlgorithmForTSP::pathVariation(Path path_) {
	int cityNo1 = Random::nextInt(0, _pathLength);
	int cityNo2 = Random::nextInt(0, _pathLength);
	int tempCity = path_.getCity(cityNo1);
	path_.setCity(cityNo1, path_.getCity(cityNo2));
	path_.setCity(cityNo2, tempCity);
	return path_;
//	/// 找出和相邻城市距离中最长的两个城市交换
//	int cityNo1 = 0;
//	int cityNo2 = 1;
//	int city1 = path_.getCity(0);
//	int	city2 = path_.getCity(1);
//	double maxDistance1 = _cityDistance[0][city1]
//			+ _cityDistance[city1][city2];
//	double maxDistance2 = _cityDistance[city1][city2]
//			+ _cityDistance[city2][path_.getCity(2)];
//	if (maxDistance1 < maxDistance2) {
//		cityNo2 = 0;
//		cityNo1 = 1;
//		int tempCity = city2;
//		city2 = city1;
//		city1 = tempCity;
//		double tempDistance = maxDistance2;
//		maxDistance2 = maxDistance1;
//		maxDistance1 = tempDistance;
//	}
//	for (unsigned i = 2; i < _pathLength-1; ++i) {
//		int city = path_.getCity(i);
//		double currDistance = _cityDistance[path_.getCity(i-1)][city]
//				+ _cityDistance[city][path_.getCity(i+1)];
//
//		if (maxDistance1 < currDistance) {
//			cityNo2 = cityNo1;
//			maxDistance2 = maxDistance1;
//			city2 = city1;
//			cityNo1 = i;
//			city1 = city;
//			maxDistance1 = currDistance;
//		} else if (maxDistance2 < currDistance) {
//			cityNo2 = i;
//			city2 = city;
//			maxDistance2 = currDistance;
//		}
//	}
//	int city = path_.getCity(_pathLength-1);
//	double currDistance = _cityDistance[path_.getCity(_pathLength-2)][city]
//			+ _cityDistance[city][0];
//
//	if (maxDistance1 < currDistance) {
//		cityNo2 = cityNo1;
//		cityNo1 = _pathLength - 1;
//		city2 = city1;
//		city1 = city;
//	} else if (maxDistance2 < currDistance) {
//		cityNo2 = _pathLength - 1;
//		city2 = city;
//	}
//
//	path_.setCity(cityNo1, city2);
//	path_.setCity(cityNo2, city1);
//
//	return path_;
}


/** \brief 更新最短距离的路径和相关计数器
 *
 * \param path_ const Path&
 * \return void
 *
 */
void GeneticAlgorithmForTSP::updateMinDistPath(const Path &path_) {
	if (_minDistancePath.getPathDistance() > path_.getPathDistance()) {
		_minDistancePath = path_;
		_minGenerationNo = _currGenerationNo;
		_minCount = 0;
	} else {
		++ _minCount;
	}
}


/** \brief 全排列（递归版本）
 *
 * \return int
 *
 */
int GeneticAlgorithmForTSP::fullPermutation() {

	_minDistancePath.setPathDistance(double(unsigned(-1)));
	_minCount = 0;

	int cnt = 0;
	vector<int> cityPut;
	for (unsigned i = 1; i <= _pathLength; ++i) {
		cityPut.push_back(i);
	}
	vector<int> cityGet;
	permutation(cityGet, cityPut, cnt);
	return cnt;
}



/** \brief 用于全排列递归算法
 *
 * \param cityGet_ vector<int>
 * \param cityPut_ vector<int>
 * \param cnt_ int&
 * \return void
 *
 */
void GeneticAlgorithmForTSP::
permutation(vector<int> cityGet_, vector<int> cityPut_, int &cnt_) {
	///形成一个排列
	if (cityGet_.size() == _pathLength) {
		Path path(cityGet_);
		/// 计算路径
		path.setPathDistance(calculatePathDistance(path));
		/// 更新最短路径
		updateMinDistPath(path);

		if (_isFullPermutationDisplay) {
			if (cnt_ == 0) {
				cout << "全排列（递归版本）：" << endl;
			}
			path.printPath();
		}
		++cnt_;
		return;
	}
	for (unsigned i = 0; i < _pathLength; ++i) {
		if (cityPut_[i] == -1) continue;
		int city = cityPut_[i];
		cityPut_[i] = -1;
		cityGet_.push_back(city);
		permutation(cityGet_, cityPut_, cnt_);
		cityGet_.pop_back();
		cityPut_[i] = city;
	}
}


/** \brief 全排列（迭代版本）
 *
 * \return int
 *
 */
int GeneticAlgorithmForTSP::fullPermutation2() {

	_minDistancePath.setPathDistance(double(unsigned(-1)));
	_minCount = 0;

	int cnt = 0;
	vector<int> cityPut;
	for (unsigned i = _pathLength; i > 0; --i) {
		cityPut.push_back(i);
	}
	vector<int> cityGet;
	stack<vector<int> > cityGetStack;
	stack<vector<int> > cityPutStack;

	cityGetStack.push(cityGet);
	cityPutStack.push(cityPut);

	while (!cityGetStack.empty()) {
		cityGet = move(cityGetStack.top()); cityGetStack.pop();
		cityPut = move(cityPutStack.top()); cityPutStack.pop();

		if (cityGet.size() == _pathLength) {
			///形成一个排列
			Path path(cityGet);
			/// 计算路径
			path.setPathDistance(calculatePathDistance(path));
			/// 更新最短路径
			updateMinDistPath(path);
			/// 显示
			if (_isFullPermutationDisplay) {
				if (cnt == 0) {
					cout << "全排列（迭代版本）：" << endl;
				}
				path.printPath();
			}
			++cnt;
			continue;
		}

		for (unsigned i = 0; i < _pathLength; ++i) {
			if (cityPut[i] == -1) continue;
			int city = cityPut[i];
			cityPut[i] = -1;
			cityGet.push_back(city);

			cityGetStack.push(cityGet);
			cityPutStack.push(cityPut);

			cityGet.pop_back();
			cityPut[i] = city;
		}
	}

	return cnt;
}
