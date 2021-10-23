package eightNumCode;
import java.util.ArrayList;

public class AStart {
	
	private static int ADDTION_COST = 1;			//����

	private BoardStatus startBS;					// ��ʼ����
	private BoardStatus endBS;						// Ŀ�����
	private BoardStatus currBS;						// ��ǰ����
	private ArrayList<BoardStatus> waitArray;		// �ȴ�����
	private ArrayList<BoardStatus> usedArray;		// �жϹ�����
	private int count = 0;							// ��¼��������
	private boolean runNoResult = false;			// �Ƿ������޽⣨����ö�٣�
	private int stepCnt;							// ·������
	private boolean detailInfo = false;				// �Ƿ���ʾϸ����Ϣ
	private long runTime;							// ����ʱ��
	
	/**
	 * ���캯��
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
	 * ִ���㷨���ִ�Ŀ��״̬
	 * @return
	 * @throws InterruptedException 
	 */
	public boolean run() {
		long startTime = System.currentTimeMillis();
		if (this.runNoResult == false) {
			/* �ж��Ƿ��н� */
			if (this.hasSolution() == false) {
				return false;
			}
		}
		
		while (true) {
			
			++count;
			
			if (this.runNoResult == true) {
				if (count % 10000 == 0) {
					System.out.println("��������: " + count);
					System.out.println("waitArray_size:" + this.waitArray.size());
					System.out.println("usedArray_size:" + this.usedArray.size());
					long currTime = System.currentTimeMillis();
					System.out.println("��ʱ: " + (currTime - startTime) + "����");
				}
			}
			
			if (this.detailInfo == true) {
				System.out.println("��������: "  +  this.count);
				System.out.println("waitArray_size:" + this.waitArray.size());
				System.out.println("usedArray_size:" + this.usedArray.size());
				this.showWaitArray();
			}
			
			/* ��ȡ�ȴ������� Fֵ��С��״̬ */
			this.currBS = this.getFMinBS();
			
			/* �޽�  */
			if (this.currBS == null) {
				System.out.println("�޽�");
				System.out.println("��������: " + count);
				System.out.println("waitArray_size:" + this.waitArray.size());
				return false;
			}
			
			if (this.detailInfo == true) {
				System.out.println("CurrBoardStatus:");
				System.out.println(this.currBS);
			}
			
			/* �н����ҵ�·�� */
			if (this.currBS.getH() == 0) {
				
				long endTime = System.currentTimeMillis();
				this.runTime = endTime - startTime;
	
				this.endBS = this.currBS;
				return true;
			}
			// ������֧
			this.deriveCurrBS();
		}
	}
	
	
	/**
	 *  �жϳ�ʼ״̬��Ŀ��״̬�Ƿ��н�
	 * ����ʼ״̬��Ŀ��״̬�������ͬ��ͬż���н⣬�����޽⣬0���⣩
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
	 * �����С��֧,������֧�����������
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
	 * ��õ�ǰ�ķ�֧����״̬�б�
	 * @return
	 * @throws InterruptedException 
	 */
	private void deriveCurrBS() {
		
		/* ���ڶ��߳� */
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

		/* ʹ�ö��̶߳��ĸ���֧����������ȥ�� */
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
		
		/* ���̴߳���,��ȴ�����ֹ,��ȡ��ȥ�غ��״̬(null�������) */
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
		
		/* ���ȥ�غ����¾���,�������ȴ����еȴ��ж� */
		if (leftBS != null) this.waitArray.add(leftBS);
		if (rightBS != null) this.waitArray.add(rightBS);
		if (upBS != null) this.waitArray.add(upBS);
		if (downBS != null) this.waitArray.add(downBS);
	}
	
	
	/**
	 * �ӿմ��ƶ�һ��
	 * @param dX
	 * @param dY
	 * @return BoardStatus
	 */
	public BoardStatus moveCurrBS(int dX, int dY) {
		
		int currSpaceX = this.currBS.getSpaceX();
		int currSpaceY = this.currBS.getSpaceY();
		int newSpaceX = currSpaceX + dX;
		int newSpaceY = currSpaceY + dY;
		
		/* ���ڵ㲻���� */
		if (this.currBS.getParent() != null) {
			if (newSpaceX == this.currBS.getParent().getSpaceX() 
					&& newSpaceY == this.currBS.getParent().getSpaceY()) {
				return null;
			}
		}
		
		/* �����½ڵ㲢�����½ڵ��һϵ������  */
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
	 * �������̵����벻ͬ�ĸ���
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
	 *  ��ʾ·������
	 * @return
	 */
	public void showPath() {
		this.stepCnt = 0;
		this.showPathPecursive(this.endBS);
	}
	
	/**
	 *  ��ʾ·���ݹ麯��
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
				System.out.println("������");
			} else if (parentBS.getSpaceX() == pathBS.getSpaceX() - 1) {
				System.out.println("������");
			} else if (parentBS.getSpaceY() == pathBS.getSpaceY() + 1) {
				System.out.println("������");
			} else if (parentBS.getSpaceY() == pathBS.getSpaceY() - 1) {
				System.out.println("������");
			}
		}
		
		System.out.println(pathBS);	
		System.out.println();
		++stepCnt;
	}
	
	/**
	 *   ���ص�������
	 * @return
	 */
	public int getCount() {
		return this.count;
	}
	
	/**
	 * ��ò���
	 * @return
	 */
	public int getStepCount() {
		return this.stepCnt;
	}
	
	/**
	 *  �������ʱ��
	 * @return
	 */
	public long getRunTime() {
		return this.runTime;
	}
	
	/**
	 * �����Ƿ��ӡ������Ϣ
	 * @param status
	 */
	public void setDetailInfo(boolean status) {
		this.detailInfo = status;
	}
	
	/**
	 * �����Ƿ������޽����
	 * @param status
	 */
	public void setRunNoResult(boolean status) {
		this.runNoResult = status;
	}
	
	
	/**
	 * ��ʾ�ȴ���չ�Ľڵ�
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
