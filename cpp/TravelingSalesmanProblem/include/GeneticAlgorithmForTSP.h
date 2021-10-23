#ifndef GENETICALGORITHMFORTSP_H
#define GENETICALGORITHMFORTSP_H

#include <string>
#include <vector>
#include "Path.h"

using std::string;
using std::vector;



/** \brief �Ŵ��㷨�⡰���������⡱
 */
class GeneticAlgorithmForTSP {

public:

/**< ���캯�� */

	GeneticAlgorithmForTSP(const string &fileName_);

/**< �������� */

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

/**< cpp�ļ����� */
	void printPopulation() const;

	void printCityDistance() const;

	void run();

	int fullPermutation();

	int fullPermutation2();

private:

/**< ˽�к��� */
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

/**< ˽�г�Ա���� */
	vector<vector<double> > _cityDistance;	/**< �����о���Ķ�ά���Գƾ���*/

	unsigned _cityNum;						/**< ������ */

	unsigned _pathLength;					/**< ·�����ȣ���������һ�� */

	unsigned _populationNum;				/**< ��Ⱥ��������·������ */

	unsigned _generationNum;				/**< �������� */

	vector<Path> _pathPopulation;			/**< ��ǰ��Ⱥ��·�����ϣ� */

	unsigned _currGenerationNo;				/**< ��ǰ���� */

	double _avgDistance;					/**< ��ǰ��Ⱥ��ƽ��·�� */

	Path _minDistancePath;					/**< ��ǰ�����·�� */

	unsigned _minGenerationNo;				/**< ���·��������ֵĴ��� */

	unsigned _minCount;						/**< ��ǰ���·��ά�ֵ������� */

	unsigned _displayNum;					/**< ÿ�����ٴ���ʾ */

	unsigned _fitnessIndex;					/**< ��Ӧ�Ȳ��������η� */

	bool _isDisplay;						/**< �Ƿ���ʾϸ����� */

	bool _isFullPermutationDisplay;			/**< �Ƿ���ʾȫ������� */
};

#endif // GENETICALGORITHMFORTSP_H
