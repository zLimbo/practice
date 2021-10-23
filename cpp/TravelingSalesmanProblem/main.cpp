#include <iostream>
#include <ctime>
#include "GeneticAlgorithmForTSP.h"		/**< 遗传算法头文件 */

using namespace std;

vector<vector<double> > cityDistance;

/** \brief 主函数
 *
 * \return int
 *
 */
int main() {

	srand(unsigned(time(nullptr)));		/**< 设置随机种子 */

	GeneticAlgorithmForTSP gaTSP("5CityInput.txt");	/**< 创建GA for TSP 类 */
	gaTSP.printCityDistance();		/**< 打印城市距离二维表 */
	cout << endl;

	gaTSP.setGenerationNum(10000);	/**< 设置遗传迭代次数 */
	gaTSP.setPopulationNum(10);		/**< 设置遗传种群物种数 */

	gaTSP.setDisplay(false, 1, false);		/**< 是否打印细节情况，每个几代打印一次,是否打印全排列细节情况 */

	for (int index = 1; index <= 4; ++index) {
		//unsigned index = 1;
		cout << "\n适应度指数 = " << index << "\t";
		gaTSP.setFitnessIndex(index);	/**< 设置适应度函数指数 */
		gaTSP.run();	/**< GA算法运行 */

		Path minDistancePath = gaTSP.getMinDistancePath();		/**< 获得最短路径 */
		cout << "最短路径可能为：" << endl;
		minDistancePath.printPath();		/**< 打印GA算法得到的最短路径（最终结果） */
		cout << " 出现在第 " << gaTSP.getMinGenerationNo() << " 代\n" << endl;

		//gaTSP.printPopulation();
	}

	cout << "\n" << endl;
//	int fullPermutationCnt2 = gaTSP.fullPermutation();		/**< 全排列遍历（递归版本） */
	int fullPermutationCnt2 = gaTSP.fullPermutation2();		/**< 全排列遍历（迭代版本） */

	Path realMinDistancePath = gaTSP.getMinDistancePath();		/**< 获得真正的最短路径 */
	cout << "\n通过全排列遍历，真正的最短路径为：" << endl;
	realMinDistancePath.printPath();	/**< 打印真正的最短路径（最终结果） */
	cout << "全排列个数 = " << fullPermutationCnt2 << endl;


	return 0;
}



