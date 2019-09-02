#ifndef _ItpMap_h
#define _ItpMap_h

#include "BinMap.h"

namespace cppalgo
{
	template <class TYPE> class ItpMap : public BinMap<TYPE>
	{
	public:
		ItpMap(int nSize = 100) : BinMap<TYPE>(nSize) { }
		~ItpMap() { }

	protected:
		bool _find(MapPos& pos) const;
	};
	
	template <class TYPE>
		bool ItpMap<TYPE>::_find(MapPos& pos) const
	{
		// TYPE�� ����Ÿ�Ը� ����....
		int mid;
		int left = 0;
		int right = m_nLen - 1;
		while (right >= left)
		{
			if (!(m_pArray[right] == m_pArray[left])) 
			{
				mid = (int)(left + (double)(pos.key - m_pArray[left])*(right-left) / (m_pArray[right] - m_pArray[left]));
				if (mid < left) mid = left;		// �������̶� ������ ��� ���� �ִ�. 
				if (mid > right) mid = right; 
			}
			else
				mid = left;   // ������ 0�� ���, left�� right���̴� ��� Ű��. �ƹ��ų� ���� 
			if (pos.key == m_pArray[mid])
			{
				pos.index = mid;
				return true;
			}
			if (pos.key > m_pArray[mid])
				left = mid + 1;
			else
				right = mid - 1;
		}
		// ��ã�� ���
		return false;
	}
};

#endif