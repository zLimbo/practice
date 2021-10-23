package eightNumCode;

import java.util.ArrayList;

/**
 * ȥ���߳�
 * @author zLimbo
 *
 */
public class RemoveDuplicateThread implements Runnable {

	private ArrayList<BoardStatus> waitArray;	// �ȴ�����
	private ArrayList<BoardStatus> usedArray;	// �жϹ��Ķ���
	private BoardStatus checkedBS;				// ���жϵ����
	
	
	/**
	 * ���캯��
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
	 * ����ȥ�غ�Ľ��,���������߳����к����Ч
	 * @return null��ʾ�ظ�����null���ظ�
	 */
	public BoardStatus uniqueBoardStatus() {
		
		return checkedBS;
	}
	
	
	/**
	 * ȥ���߳����У����ڱ�������״̬�鿴�Ƿ��ظ�
	 * @see java.lang.Runnable#run()
	 */
	@Override
	public void run() {
		/* �ж�״̬Ϊ null, ������ȥ�� */
		if (checkedBS == null) {
			return;
		}
		// �� used������ȥ��
		for (BoardStatus iterBS: usedArray) {
			if (iterBS.equalValue(checkedBS)) {
				checkedBS = null;
				return;
			}
		}
		// ��wait������ȥ��
		for (BoardStatus iterBS: waitArray) {
			if (iterBS.equalValue(checkedBS)) {
				checkedBS = null;
				return;
			}
		}
	}
}
