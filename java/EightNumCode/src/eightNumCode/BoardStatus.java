package eightNumCode;

/**
 * 状态类
 * @author Administrator
 */
public class BoardStatus{

	public static int ROW_LEN = 3;	//数码盘为 3行3列的局面
	public static int COL_LEN = 3;
	public static int SPACE = 0;	// 空格用 0 代替
	
	private int[][] board;			// 局面
	private int g;					// 启发式函数
	private int h;					// 代价函数
	private int spaceX;				// 空格位置x值（即0位置）
	private int spaceY;				// 空格位置y值
	private BoardStatus parent;		// 父亲节点，用于递归打印路径
	
	/**
	 * 构造函数
	 * @param board	3X3数组
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
	 * 构造函数
	 * @param bs BoardStatus类，
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
	 * 判断两个棋盘状态的棋盘面值是否相等
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
	 * 字符串表示
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
	
	
	/* 以下是一系列 setter 和 getter 函数 */
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
