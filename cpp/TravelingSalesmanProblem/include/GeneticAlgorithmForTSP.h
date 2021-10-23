#ifndef GENETICALGORITHMFORTSP_H
#define GENETICALGORITHMFORTSP_H

#include <string>
#include <vector>
#include "Path.h"

using std::string;
using std::vector;



/** \brief 遗传算法解“旅行商问题”
 */
class GeneticAlgorithmForTSP {

public:

/**< 构造函数 */

	GeneticAlgorithmForTSP(const string &fileName_);

/**< 内联函数 */

	vector<vector<double> > getCityDistance() const { return _cityDistance; }

	void setPopulationNum(unsigned populationNum_) { _populationNum = populationNum_; }

	unsigned getPopulationNum() const { return _populationNum; }

	void setGenerationNum(unsigned generationNum_) { _generationNum = generationNum_; }

	unsigned getGenerationNum() const { return _generationNum; }

	unsigned getCityNum() const { return _cityNum; }

	void setFitnessIndex(unsigned index_) { _fitnessIndex = index_; }

	Path getMinDistancePath() const { return _minDistancePath; }

	unsigned getMinGenerationNo() const { return _minGenerationNo; }

	unsigned getMinCount() const { return _minCount; }

	void setDisplay(bool isDisplay_, unsigned displayNum_ = 1, bool isFP_ = false) {
		if ((_isDisplay = isDisplay_) && displayNum_ != 0) { _displayNum = displayNum_; }
		if (isFP_) { _isFullPermutationDisplay = isFP_; }
	};

/**< cpp文件函数 */
	void printPopulation() const;

	void printCityDistance() const;

	void run();

	int fullPermutation();

	int fullPermutation2();

private:

/**< 私有函数 */
	void initial();

	void calculateFitness();

	double calculatePathDistance(const Path &path) const;

	double fitnessFunc(double pathReci_) const;

	void geneticPopulation();

	Path pathVariation(Path path_);

	void pathCross(Path &path1_, Path &path2_);

	Path& getPathByFitness();

	void updateMinDistPath(const Path &path_);

	void permutation(vector<int> cityGet_, vector<int> cityPut_, int &cnt_);

private:

/**< 私有成员变量 */
	vector<vector<double> > _cityDistance;	/**< 各城市距离的二维表（对称矩阵）*/

	unsigned _cityNum;						/**< 城市数 */

	unsigned _pathLength;					/**< 路径长度（城市数减一） */

	unsigned _populationNum;				/**< 种群物种数（路径数） */

	unsigned _generationNum;				/**< 迭代次数 */

	vector<Path> _pathPopulation;			/**< 当前种群（路径集合） */

	unsigned _currGenerationNo;				/**< 当前代数 */

	double _avgDistance;					/**< 当前种群的平均路径 */

	Path _minDistancePath;					/**< 当前的最短路径 */

	unsigned _minGenerationNo;				/**< 最短路径最早出现的代数 */

	unsigned _minCount;						/**< 当前最短路径维持迭代次数 */

	unsigned _displayNum;					/**< 每个多少代显示 */

	unsigned _fitnessIndex;					/**< 适应度参数，几次方 */

	bool _isDisplay;						/**< 是否显示细节情况 */

	bool _isFullPermutationDisplay;			/**< 是否显示全排列情况 */
};

#endif // GENETICALGORITHMFORTSP_H
