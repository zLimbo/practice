import eightNumCode.*;

public class Test {

	public static void main(String[] args) {
		
		int[][] startArr = {	
				
				{5, 2, 8},	// 有解
				{3, 0, 1},
				{6, 7, 4},
				
//				{1, 2, 3},	// 无解
//				{4, 0, 8},
//				{7, 6, 5},
				
//				{1, 2, 3},	// 有解
//				{4, 0, 5},
//				{6, 8, 7},				
		};
		
		
		int[][] endArr = {
				
				{1, 2, 3},
				{8, 0, 4},
				{7, 6, 5},
		};
		
		// 初始化
		BoardStatus startBS = new BoardStatus(startArr);
		BoardStatus endBS = new BoardStatus(endArr);
		AStart aStart = new AStart(startBS, endBS);
		
//		aStart.setDetailInfo(true);		// 是否打印细节信息
//		aStart.setRunNoResult(true);	// 是否使用暴力枚举方法计算无解情况
		boolean hasSolution = aStart.run();	// A星算法运行
		
		System.out.println("起始状态：\n" + startBS);
		System.out.println("目标状态：\n" + endBS);
		
		if (hasSolution == false) {
			System.out.println("无解");
		} else {
			System.out.println("移动方案为：");
			aStart.showPath();	// 打印路径
			System.out.println("共 " + aStart.getStepCount() + "步");
			System.out.println("耗时： " + aStart.getRunTime() + "毫秒");
			System.out.println("迭代次数: " + aStart.getCount());
		}
	}
}
