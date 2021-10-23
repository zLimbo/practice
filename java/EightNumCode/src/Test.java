import eightNumCode.*;

public class Test {

	public static void main(String[] args) {
		
		int[][] startArr = {	
				
				{5, 2, 8},	// �н�
				{3, 0, 1},
				{6, 7, 4},
				
//				{1, 2, 3},	// �޽�
//				{4, 0, 8},
//				{7, 6, 5},
				
//				{1, 2, 3},	// �н�
//				{4, 0, 5},
//				{6, 8, 7},				
		};
		
		
		int[][] endArr = {
				
				{1, 2, 3},
				{8, 0, 4},
				{7, 6, 5},
		};
		
		// ��ʼ��
		BoardStatus startBS = new BoardStatus(startArr);
		BoardStatus endBS = new BoardStatus(endArr);
		AStart aStart = new AStart(startBS, endBS);
		
//		aStart.setDetailInfo(true);		// �Ƿ��ӡϸ����Ϣ
//		aStart.setRunNoResult(true);	// �Ƿ�ʹ�ñ���ö�ٷ��������޽����
		boolean hasSolution = aStart.run();	// A���㷨����
		
		System.out.println("��ʼ״̬��\n" + startBS);
		System.out.println("Ŀ��״̬��\n" + endBS);
		
		if (hasSolution == false) {
			System.out.println("�޽�");
		} else {
			System.out.println("�ƶ�����Ϊ��");
			aStart.showPath();	// ��ӡ·��
			System.out.println("�� " + aStart.getStepCount() + "��");
			System.out.println("��ʱ�� " + aStart.getRunTime() + "����");
			System.out.println("��������: " + aStart.getCount());
		}
	}
}
