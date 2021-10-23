package eightNumCode;

import java.util.ArrayList;

/**
 * 去重线程
 * @author zLimbo
 *
 */
public class RemoveDuplicateThread implements Runnable {

	private ArrayList<BoardStatus> waitArray;	// 等待队列
	private ArrayList<BoardStatus> usedArray;	// 判断过的队列
	private BoardStatus checkedBS;				// 待判断的情况
	
	
	/**
	 * 构造函数
	 * @param waitArray
	 * @param usedArray
	 * @param checkedBS
	 */
	public RemoveDuplicateThread(
			ArrayList<BoardStatus> waitArray, 
			ArrayList<BoardStatus> usedArray,
			BoardStatus checkedBS) 
	{
		this.waitArray = waitArray;
		this.usedArray = usedArray;
		this.checkedBS = checkedBS;
	}
	
	
	/**
	 * 返回去重后的结果,但必须在线程运行后才有效
	 * @return null表示重复，非null则不重复
	 */
	public BoardStatus uniqueBoardStatus() {
		
		return checkedBS;
	}
	
	
	/**
	 * 去重线程运行，用于遍历已有状态查看是否重复
	 * @see java.lang.Runnable#run()
	 */
	@Override
	public void run() {
		/* 判断状态为 null, 则无须去重 */
		if (checkedBS == null) {
			return;
		}
		// 在 used队列中去重
		for (BoardStatus iterBS: usedArray) {
			if (iterBS.equalValue(checkedBS)) {
				checkedBS = null;
				return;
			}
		}
		// 在wait队列中去重
		for (BoardStatus iterBS: waitArray) {
			if (iterBS.equalValue(checkedBS)) {
				checkedBS = null;
				return;
			}
		}
	}
}
