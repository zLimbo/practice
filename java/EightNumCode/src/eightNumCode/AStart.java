package eightNumCode;
import java.util.ArrayList;

public class AStart {
	
	private static int ADDTION_COST = 1;			//代价

	private BoardStatus startBS;					// 开始局面
	private BoardStatus endBS;						// 目标局面
	private BoardStatus currBS;						// 当前局面
	private ArrayList<BoardStatus> waitArray;		// 等待队列
	private ArrayList<BoardStatus> usedArray;		// 判断过队列
	private int count = 0;							// 记录迭代次数
	private boolean runNoResult = false;			// 是否运行无解（暴力枚举）
	private int stepCnt;							// 路径步数
	private boolean detailInfo = false;				// 是否显示细节信息
	private long runTime;							// 运行时间
	
	/**
	 * 构造函数
	 * @param startBS
	 * @param endBS
	 */
	public AStart(BoardStatus startBS, BoardStatus endBS) {
		
		this.startBS = new BoardStatus(startBS);
		this.endBS = new BoardStatus(endBS);
		this.waitArray = new ArrayList<BoardStatus>();
		this.usedArray = new ArrayList<BoardStatus>();
		
		this.startBS.setH(this.diffOfTwoBS(this.startBS, this.endBS));
		this.startBS.setG(0);
		this.startBS.setParent(null);
		this.waitArray.add(this.startBS);
	}
	
	
	/**
	 * 执行算法，抵达目标状态
	 * @return
	 * @throws InterruptedException 
	 */
	public boolean run() {
		long startTime = System.currentTimeMillis();
		if (this.runNoResult == false) {
			/* 判断是否有解 */
			if (this.hasSolution() == false) {
				return false;
			}
		}
		
		while (true) {
			
			++count;
			
			if (this.runNoResult == true) {
				if (count % 10000 == 0) {
					System.out.println("迭代次数: " + count);
					System.out.println("waitArray_size:" + this.waitArray.size());
					System.out.println("usedArray_size:" + this.usedArray.size());
					long currTime = System.currentTimeMillis();
					System.out.println("耗时: " + (currTime - startTime) + "毫秒");
				}
			}
			
			if (this.detailInfo == true) {
				System.out.println("迭代次数: "  +  this.count);
				System.out.println("waitArray_size:" + this.waitArray.size());
				System.out.println("usedArray_size:" + this.usedArray.size());
				this.showWaitArray();
			}
			
			/* 获取等待队列中 F值最小的状态 */
			this.currBS = this.getFMinBS();
			
			/* 无解  */
			if (this.currBS == null) {
				System.out.println("无解");
				System.out.println("迭代次数: " + count);
				System.out.println("waitArray_size:" + this.waitArray.size());
				return false;
			}
			
			if (this.detailInfo == true) {
				System.out.println("CurrBoardStatus:");
				System.out.println(this.currBS);
			}
			
			/* 有解且找到路径 */
			if (this.currBS.getH() == 0) {
				
				long endTime = System.currentTimeMillis();
				this.runTime = endTime - startTime;
	
				this.endBS = this.currBS;
				return true;
			}
			// 派生分支
			this.deriveCurrBS();
		}
	}
	
	
	/**
	 *  判断初始状态与目标状态是否有解
	 * （初始状态和目标状态的逆序对同奇同偶则有解，否则无解，0除外）
	 * @return
	 */
	private boolean hasSolution() {
		int arrLen = BoardStatus.COL_LEN * BoardStatus.ROW_LEN - 1;
		int[] startJudgeArr = new int[arrLen];
		int[] endJudgeArr = new int[arrLen];
		int startJudgeCnt = 0;
		int endJudgeCnt = 0;
		int startArrCnt = 0;
		int endArrCnt = 0;
		for (int row = 0; row < BoardStatus.ROW_LEN; ++row) {
			for (int col = 0; col < BoardStatus.COL_LEN; ++col) {
				if (this.startBS.getBoard(row,  col) != BoardStatus.SPACE) {
					startJudgeArr[startArrCnt++] = this.startBS.getBoard(row, col);
				}
				if (this.endBS.getBoard(row, col) != BoardStatus.SPACE) {
					endJudgeArr[endArrCnt++] = this.endBS.getBoard(row, col);
				}
			}
		}
		for (int i = 0; i < arrLen; ++i) {
			for (int j = 0; j < i; ++j) {
				if (startJudgeArr[j] < startJudgeArr[i]) {
					++startJudgeCnt;
				}
				if (endJudgeArr[j] < endJudgeArr[i]) {
					++endJudgeCnt;
				}
			}
		}
		if ((startJudgeCnt - endJudgeCnt) % 2 == 0) {
			return true;
		} else {
			return false;
		}
	}
	
	/**
	 * 获得最小分支,弹出分支区，进入旧区
	 * @return
	 */
	private BoardStatus getFMinBS() {	
		if (this.waitArray.isEmpty()) {
			return null;
		}
		BoardStatus fMinBS = this.waitArray.get(this.waitArray.size()-1);
		for (int i = this.waitArray.size()-2; i >= 0; --i) {
			if (this.waitArray.get(i).getF() < fMinBS.getF()) {
				fMinBS = this.waitArray.get(i);
			}
		}
		this.waitArray.remove(fMinBS);
		this.usedArray.add(fMinBS);
		return fMinBS;
	}
	
	
	/**
	 * 获得当前的分支期盼状态列表
	 * @return
	 * @throws InterruptedException 
	 */
	private void deriveCurrBS() {
		
		/* 用于多线程 */
		RemoveDuplicateThread leftRDT = null;
		RemoveDuplicateThread rightRDT = null;
		RemoveDuplicateThread upRDT = null;
		RemoveDuplicateThread downRDT = null;
		Thread leftTh = null;
		Thread rightTh = null;
		Thread upTh = null;
		Thread downTh = null;
		BoardStatus leftBS = null;
		BoardStatus rightBS = null;
		BoardStatus upBS = null;
		BoardStatus downBS = null;
		
		
		int currSpaceX = this.currBS.getSpaceX();
		int currSpaceY = this.currBS.getSpaceY();

		/* 使用多线程对四个分支进行搜索和去重 */
		if (currSpaceX >= 1) {
			downRDT = new RemoveDuplicateThread(
					this.waitArray, this.usedArray, this.moveCurrBS(-1, 0));
			downTh = new Thread(downRDT, "down");
			downTh.start();
		}
		if (currSpaceX <= 1) {
			upRDT = new RemoveDuplicateThread(
					this.waitArray, this.usedArray, this.moveCurrBS(+1, 0));
			upTh = new Thread(upRDT, "up");
			upTh.start();
		}
		if (currSpaceY >= 1) {
			leftRDT = new RemoveDuplicateThread(
					this.waitArray, this.usedArray, this.moveCurrBS(0, -1));
			leftTh = new Thread(leftRDT, "left");
			leftTh.start();
		}
		if (currSpaceY <= 1) {
			rightRDT = new RemoveDuplicateThread(
					this.waitArray, this.usedArray, this.moveCurrBS(0, +1));
			rightTh = new Thread(rightRDT, "right");
			rightTh.start();
		}
		
		/* 若线程存在,则等待其终止,并取出去重后的状态(null或新情况) */
		try {
			if (leftTh != null) {
				leftTh.join();
				leftBS = leftRDT.uniqueBoardStatus();
			}
			if (rightTh != null) {
				rightTh.join();
				rightBS = rightRDT.uniqueBoardStatus();
			}
			if (upTh != null) {
				upTh.join();
				upBS = upRDT.uniqueBoardStatus();
			}
			if (downTh != null) {
				downTh.join();
				downBS = downRDT.uniqueBoardStatus();
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		/* 如果去重后有新局面,则将其加入等待队列等待判定 */
		if (leftBS != null) this.waitArray.add(leftBS);
		if (rightBS != null) this.waitArray.add(rightBS);
		if (upBS != null) this.waitArray.add(upBS);
		if (downBS != null) this.waitArray.add(downBS);
	}
	
	
	/**
	 * 从空处移动一步
	 * @param dX
	 * @param dY
	 * @return BoardStatus
	 */
	public BoardStatus moveCurrBS(int dX, int dY) {
		
		int currSpaceX = this.currBS.getSpaceX();
		int currSpaceY = this.currBS.getSpaceY();
		int newSpaceX = currSpaceX + dX;
		int newSpaceY = currSpaceY + dY;
		
		/* 父节点不派生 */
		if (this.currBS.getParent() != null) {
			if (newSpaceX == this.currBS.getParent().getSpaceX() 
					&& newSpaceY == this.currBS.getParent().getSpaceY()) {
				return null;
			}
		}
		
		/* 产生新节点并设置新节点的一系列属性  */
		BoardStatus newBS = new BoardStatus(this.currBS);
		newBS.setBoard(currSpaceX, currSpaceY, this.currBS.getBoard(newSpaceX, newSpaceY));
		newBS.setBoard(newSpaceX, newSpaceY, BoardStatus.SPACE);
		newBS.setSpaceXY(newSpaceX, newSpaceY);
		newBS.setH(this.diffOfTwoBS(newBS, this.endBS));	
		newBS.setG(this.currBS.getG() + ADDTION_COST);
		newBS.setParent(this.currBS);
		
		return newBS;
	}
	

	/**
	 * 两个棋盘的数码不同的个数
	 * @param lhsBS
	 * @param rhsBS
	 * @return
	 */
	public int diffOfTwoBS(BoardStatus lhsBS, BoardStatus rhsBS) {
		
		int diffNum = 0;
		for (int row = 0; row < BoardStatus.ROW_LEN; ++row) {
			for (int col = 0; col < BoardStatus.COL_LEN; ++col) {
				if (lhsBS.getBoard(row, col) != rhsBS.getBoard(row, col)) {
					++diffNum;
				}
			}
		}
		return diffNum;
	}
	
	
	/**
	 *  显示路径函数
	 * @return
	 */
	public void showPath() {
		this.stepCnt = 0;
		this.showPathPecursive(this.endBS);
	}
	
	/**
	 *  显示路径递归函数
	 * @param bs
	 */
	public void showPathPecursive(BoardStatus pathBS) {
		if (pathBS == null) {
			return;
		}
		
		BoardStatus parentBS = pathBS.getParent();
		this.showPathPecursive(parentBS);
		
		if (parentBS != null) {
			if (parentBS.getSpaceX() == pathBS.getSpaceX() + 1) {
				System.out.println("向上移");
			} else if (parentBS.getSpaceX() == pathBS.getSpaceX() - 1) {
				System.out.println("向下移");
			} else if (parentBS.getSpaceY() == pathBS.getSpaceY() + 1) {
				System.out.println("向左移");
			} else if (parentBS.getSpaceY() == pathBS.getSpaceY() - 1) {
				System.out.println("向右移");
			}
		}
		
		System.out.println(pathBS);	
		System.out.println();
		++stepCnt;
	}
	
	/**
	 *   返回迭代次数
	 * @return
	 */
	public int getCount() {
		return this.count;
	}
	
	/**
	 * 获得步数
	 * @return
	 */
	public int getStepCount() {
		return this.stepCnt;
	}
	
	/**
	 *  获得运行时间
	 * @return
	 */
	public long getRunTime() {
		return this.runTime;
	}
	
	/**
	 * 设置是否打印具体信息
	 * @param status
	 */
	public void setDetailInfo(boolean status) {
		this.detailInfo = status;
	}
	
	/**
	 * 设置是否运行无解情况
	 * @param status
	 */
	public void setRunNoResult(boolean status) {
		this.runNoResult = status;
	}
	
	
	/**
	 * 显示等待扩展的节点
	 * 
	 */
	public void showWaitArray() {
		int waitArrSize = this.waitArray.size();
		for (int i = 0; i < waitArrSize; ++i) {
			BoardStatus tmpBS = this.waitArray.get(i);
			System.out.format("#%2d=%2d+%2d  ",
					tmpBS.getF(), tmpBS.getG(), tmpBS.getH());
		}
		System.out.println();
		for (int row = 0; row < BoardStatus.ROW_LEN; ++row) {
			for (int i = 0; i < waitArrSize; ++i) {
				System.out.print("  ");
				BoardStatus tmpBS = this.waitArray.get(i);
				for (int col = 0; col < BoardStatus.COL_LEN; ++col) {
					System.out.print(tmpBS.getBoard(row, col) + " ");
				}
				System.out.print("   ");
			}
			System.out.println();
		}
		for (int i = 0; i < waitArrSize; ++i) {
			System.out.print(" -------   ");
		}
		System.out.println();
	}
	
}
