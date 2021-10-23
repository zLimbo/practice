#include <iostream>
#include <ctime>
#include "GeneticAlgorithmForTSP.h"		/**< �Ŵ��㷨ͷ�ļ� */

using namespace std;

vector<vector<double> > cityDistance;

/** \brief ������
 *
 * \return int
 *
 */
int main() {

	srand(unsigned(time(nullptr)));		/**< ����������� */

	GeneticAlgorithmForTSP gaTSP("5CityInput.txt");	/**< ����GA for TSP �� */
	gaTSP.printCityDistance();		/**< ��ӡ���о����ά�� */
	cout << endl;

	gaTSP.setGenerationNum(10000);	/**< �����Ŵ��������� */
	gaTSP.setPopulationNum(10);		/**< �����Ŵ���Ⱥ������ */

	gaTSP.setDisplay(false, 1, false);		/**< �Ƿ��ӡϸ�������ÿ��������ӡһ��,�Ƿ��ӡȫ����ϸ����� */

	for (int index = 1; index <= 4; ++index) {
		//unsigned index = 1;
		cout << "\n��Ӧ��ָ�� = " << index << "\t";
		gaTSP.setFitnessIndex(index);	/**< ������Ӧ�Ⱥ���ָ�� */
		gaTSP.run();	/**< GA�㷨���� */

		Path minDistancePath = gaTSP.getMinDistancePath();		/**< ������·�� */
		cout << "���·������Ϊ��" << endl;
		minDistancePath.printPath();		/**< ��ӡGA�㷨�õ������·�������ս���� */
		cout << " �����ڵ� " << gaTSP.getMinGenerationNo() << " ��\n" << endl;

		//gaTSP.printPopulation();
	}

	cout << "\n" << endl;
//	int fullPermutationCnt2 = gaTSP.fullPermutation();		/**< ȫ���б������ݹ�汾�� */
	int fullPermutationCnt2 = gaTSP.fullPermutation2();		/**< ȫ���б����������汾�� */

	Path realMinDistancePath = gaTSP.getMinDistancePath();		/**< ������������·�� */
	cout << "\nͨ��ȫ���б��������������·��Ϊ��" << endl;
	realMinDistancePath.printPath();	/**< ��ӡ���������·�������ս���� */
	cout << "ȫ���и��� = " << fullPermutationCnt2 << endl;


	return 0;
}



