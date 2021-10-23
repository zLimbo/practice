package eightNumCode;

/**
 * ״̬��
 * @author Administrator
 */
public class BoardStatus{

	public static int ROW_LEN = 3;	//������Ϊ 3��3�еľ���
	public static int COL_LEN = 3;
	public static int SPACE = 0;	// �ո��� 0 ����
	
	private int[][] board;			// ����
	private int g;					// ����ʽ����
	private int h;					// ���ۺ���
	private int spaceX;				// �ո�λ��xֵ����0λ�ã�
	private int spaceY;				// �ո�λ��yֵ
	private BoardStatus parent;		// ���׽ڵ㣬���ڵݹ��ӡ·��
	
	/**
	 * ���캯��
	 * @param board	3X3����
	 */
	public BoardStatus(int[][] board) {
		this.board = new int[ROW_LEN][COL_LEN];
		for (int row = 0; row < ROW_LEN; ++row) {
			for (int col = 0; col < COL_LEN; ++col) {
				this.board[row][col] = board[row][col];
				if (board[row][col] == SPACE) {
					spaceX = row;
					spaceY = col;
				}
			}
		}
	} 
	
	/**
	 * ���캯��
	 * @param bs BoardStatus�࣬
	 */
	public BoardStatus(BoardStatus bs) {
		this.board = new int[ROW_LEN][COL_LEN];
		for (int row = 0; row < ROW_LEN; ++row) {
			for (int col = 0; col < COL_LEN; ++col) {
				this.board[row][col] = bs.board[row][col];
			}
		}
		this.spaceX = bs.spaceX;
		this.spaceY = bs.spaceY;
		this.g = bs.g;
		this.h = bs.h;
		this.parent = bs.parent;
	} 
	
	
	/**
	 * �ж���������״̬��������ֵ�Ƿ����
	 * @param bs
	 * @return
	 */
	public boolean equalValue(BoardStatus bs) {
		for (int row = 0; row < ROW_LEN; ++row) {
			for (int col = 0; col < COL_LEN; ++col) {
				if (this.board[row][col] != bs.board[row][col]) {
					return false;
				}
			}
		}
		return true;
	}
	
	
	/**
	 * �ַ�����ʾ
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		StringBuilder strBuilder = new StringBuilder();
		strBuilder.append("------" + getF() 
				+ "=" + getG() + "+" + getH() + "\n");
		for (int row = 0; row < ROW_LEN; ++row) {
			for (int col = 0; col < COL_LEN; ++col) {
				strBuilder.append(this.board[row][col] + " ");
			}
			strBuilder.append("\n");
		}
		strBuilder.append("------");
		return strBuilder.toString();
	}
	
	
	/* ������һϵ�� setter �� getter ���� */
	public void setParent(BoardStatus bs) {
		this.parent = bs;
	}
	
	public BoardStatus getParent() {
		return this.parent;
	}
	
	public int getBoard(int x, int y) {
		return this.board[x][y];
	}
	
	public void setBoard(int x, int y, int val) {
		this.board[x][y] = val;
	}
	
	public int getF() {
		return this.g + this.h;
	}
	
	public int getG() {
		return this.g;
	}
	
	public void setG(int g) {
		this.g = g;
	}
	
	public void setH(int h) {
		this.h = h;
	}
	
	public int getH() {
		return this.h;
	}
	
	public void setSpaceXY(int spaceX, int spaceY) {
		this.spaceX = spaceX;
		this.spaceY = spaceY;
	}
	
	public void setSpaceX(int spaceX) {
		this.spaceX = spaceX;
	}
	
	public int getSpaceX() {
		return this.spaceX;
	}
	
	public void setSpaceY(int spaceY) {
		this.spaceY = spaceY;
	}
	
	public int getSpaceY() {
		return this.spaceY;
	}
	
	
}
