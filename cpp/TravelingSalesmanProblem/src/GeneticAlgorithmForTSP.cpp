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


/** \brief ���캯��
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


/** \brief �㷨����
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


/** \brief ��ʼ��
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


/** \brief ������Ӧ��
 *
 * \return void
 *
 */
void GeneticAlgorithmForTSP::calculateFitness() {
	double totalReci = 0;
	vector<double> pathReci(_populationNum);

	for (unsigned pathNo = 0; pathNo < _populationNum; ++pathNo) {
		Path &path = _pathPopulation[pathNo];
		/// ����·��
		double pathDistance = calculatePathDistance(path);
		/// ����·��
		path.setPathDistance(pathDistance);
		/// ����·���ĵ����͵���֮��
		pathReci[pathNo] = fitnessFunc(1.0d / pathDistance );
		totalReci += pathReci[pathNo];
	}
	/// ���ݵ�������Ӧ��
	for (unsigned pathNo = 0; pathNo < _populationNum; ++pathNo) {
		_pathPopulation[pathNo].setFitness(pathReci[pathNo] / totalReci);
		/// �����·�߱Ƚϣ��������̣������Ϊ��
		updateMinDistPath(_pathPopulation[pathNo]);
	}
}


/** \brief ��·��������Ϊ��������Ӧ�Ⱥ���
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


/** \brief ����ĳһ·���ĳ���
 *
 * \param path const Path&
 * \return double
 *
 */
double GeneticAlgorithmForTSP::calculatePathDistance(const Path &path) const {
	/// ����·��
	double pathDistance = _cityDistance[0][path.getCity(0)];
	for (size_t i = 0; i < _pathLength-1; ++i) {
		pathDistance += _cityDistance[path.getCity(i)][path.getCity(i+1)];
	}
	pathDistance += _cityDistance[path.getCity(_pathLength-1)][0];
	/// ����
	return pathDistance;
}


/** \brief ��ӡ��ǰ��Ⱥ��·����
 *
 * \return void
 *
 */
void GeneticAlgorithmForTSP::printPopulation() const {
	cout << "------> �� " << _currGenerationNo << " ����Ⱥ" << endl;
	for (const Path &path: _pathPopulation) {
		path.printPath();
	}
}


/** \brief ��ӡ���о����ά��
 *
 * \return void
 *
 */
void GeneticAlgorithmForTSP::printCityDistance() const {
	cout << "  ";
	for (unsigned city = 0; city < _cityNum; ++city) {
		cout << "��" << city << "��";
	}
	cout << endl;
	for (unsigned row = 0; row < _cityNum; ++row) {
		cout << "��" << row << "��";
		for (unsigned col = 0; col < _cityNum; ++col) {
			std::cout << fixed << setprecision(2) << _cityDistance[row][col] << " ";
		}
		std::cout << std::endl;
	}
}


/** \brief ����Ŵ�����������Ⱥ
 *
 * \return void
 *
 */
void GeneticAlgorithmForTSP::geneticPopulation() {
	vector<Path> newPathPopulation;
	unsigned count = 0;
	while (count < _populationNum) {
		if (Random::nextDouble() < 0.5d) {	/// ����
			newPathPopulation.push_back(pathVariation(getPathByFitness()));
			++count;
		} else {	/// ����
			Path path1 = getPathByFitness();
			Path path2 = getPathByFitness();
			pathCross(path1, path2);
			newPathPopulation.push_back(path1);
			newPathPopulation.push_back(path2);
			count += 2;
		}
	}
	/// ����Ⱥ�������Ⱥ
	swap(newPathPopulation, _pathPopulation);
	calculateFitness();
}


/** \brief ����Ӧ�����ѡ����Ⱥ�е�ĳһ·��
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


/** \brief ����
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
	int crossLength = cityEndNo - cityBegNo;	/// ���泤��
	/// ��ȡ����Ƭ��
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

	for (int i = 0; i < crossLength; ++i) {		/// ȥ��
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
	/// ����
	path1Citys.insert(path1Citys.begin()+cityBegNo, crossPiece2.begin(), crossPiece2.end());
	path2Citys.insert(path2Citys.begin()+cityBegNo, crossPiece1.begin(), crossPiece1.end());
//
//	path1_.printPath();
//	path2_.printPath();
//	cout << "#######################################" << endl;
}


/** \brief �ɵ�ǰ·����������Ӵ�
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
//	/// �ҳ������ڳ��о���������������н���
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


/** \brief ������̾����·������ؼ�����
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


/** \brief ȫ���У��ݹ�汾��
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



/** \brief ����ȫ���еݹ��㷨
 *
 * \param cityGet_ vector<int>
 * \param cityPut_ vector<int>
 * \param cnt_ int&
 * \return void
 *
 */
void GeneticAlgorithmForTSP::
permutation(vector<int> cityGet_, vector<int> cityPut_, int &cnt_) {
	///�γ�һ������
	if (cityGet_.size() == _pathLength) {
		Path path(cityGet_);
		/// ����·��
		path.setPathDistance(calculatePathDistance(path));
		/// �������·��
		updateMinDistPath(path);

		if (_isFullPermutationDisplay) {
			if (cnt_ == 0) {
				cout << "ȫ���У��ݹ�汾����" << endl;
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


/** \brief ȫ���У������汾��
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
			///�γ�һ������
			Path path(cityGet);
			/// ����·��
			path.setPathDistance(calculatePathDistance(path));
			/// �������·��
			updateMinDistPath(path);
			/// ��ʾ
			if (_isFullPermutationDisplay) {
				if (cnt == 0) {
					cout << "ȫ���У������汾����" << endl;
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
