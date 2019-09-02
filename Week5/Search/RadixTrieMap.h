#ifndef _RadixTrieMap_h
#define _RadixTrieMap_h

#include "../Tree/BinaryTree.h"

namespace cppalgo
{
	// RadixTrieMap�� TYPE�� ��Ʈ�� �ٷ� �� �ִ� Ÿ�Ը� �����ϴ�. 
	// �ߺ��� �� ����. 
	template <class TYPE> class RadixTrieMap : public BinaryTree<TYPE>
	{
	public:
		RadixTrieMap() : BinaryTree<TYPE>() { m_nCount = 0; }
		~RadixTrieMap() {}
		// utilities
		long GetCount() const { return m_nCount; }
		long IsEmpty() const { return m_nCount == 0; }
		void RemoveAll() { BinaryTree<TYPE>::RemoveAll(); m_nCount = 0; }
		// operations
		bool Find(const TYPE& key, TYPE& value) const;
		bool Insert(const TYPE& value);
		bool Remove(const TYPE& key);
	protected:
		unsigned long bits(const TYPE& x, unsigned long k, unsigned long j) const
		{
			return (x >> k) & ~(~0 << j);
		}
		bool IsBranch(Node *p) const
		{
			return (p->pLeft != m_pNodeTail || p->pRight != m_pNodeTail);
		}
		long m_nCount;
	};

	template <class TYPE>
		bool RadixTrieMap<TYPE>::Find(const TYPE& key, TYPE& value) const
	{
		Node *t;
		unsigned b = 0;
		t = m_pNodeHead->pLeft;
		while (IsBranch(t))
		{
			t = bits(key, b++, 1) ? t->pRight : t->pLeft;
		}
		if (t->data == key) 
		{
			value = t->data;
			return true;
		}
		else
			return false;
	}

	template <class TYPE>
		bool RadixTrieMap<TYPE>::Insert(const TYPE& value)
	{
		unsigned b = 0;
		Node *gp, *p, *t;

		gp = p = m_pNodeHead;
		t = m_pNodeHead->pLeft;
		while (t != m_pNodeTail)
		{
			if (value == t->data) return false;  // ����Ű �������� ����. 
			gp = p;
			p = t;
			t = bits(value, b++, 1) ? t->pRight : t->pLeft;
		}
		b--;
		if (p == m_pNodeHead)  // ���1: ù����Ÿ 
		{
			t = new Node;
			t->pLeft = t->pRight = m_pNodeTail;
			t->data = value;
			p->pLeft = t;
		}
		else if (IsBranch(p))  // ���2: �θ� �б��� 
		{
			t = new Node;
			t->pLeft = t->pRight = m_pNodeTail;
			t->data = value;
			if (bits(value, b, 1)) p->pRight = t;
			else p->pLeft = t;
		}
		else	// ��� 3: �θ� ������� 
		{
			do
			{
				t = new Node;
				t->pLeft = t->pRight = m_pNodeTail;
				if (gp != m_pNodeHead && bits(value, b-1, 1)) gp->pRight = t;
				else gp->pLeft = t;
				gp = t;
				b++;
			} while (bits(value, b-1, 1) == bits(p->data, b-1, 1));

			t = new Node;
			t->data = value;
			t->pLeft = t->pRight = m_pNodeTail;
			if (bits(value, b-1, 1)) 
			{
				gp->pRight = t;
				gp->pLeft = p;
			}
			else
			{
				gp->pLeft = t;
				gp->pRight = p;
			}
		}

		m_nCount++;
		return true;
	}
	
	template <class TYPE>
		bool RadixTrieMap<TYPE>::Remove(const TYPE& key)
	{
		Node *delp, *del, *sib, *cp;
		unsigned b = 0, cpb = 0;
		
		cp = delp = m_pNodeHead;
		del = m_pNodeHead->pLeft;
		while (IsBranch(del))
		{
			delp = del;
			if (del->pLeft != m_pNodeTail && del->pRight != m_pNodeTail && (IsBranch(del->pLeft) || IsBranch(del->pRight)))
			{	// �ڽ��� �ΰ��� ��� : cp�� �ȴ�. 
				cp = del;
				cpb = b;
			}
			del = bits(key, b++, 1) ? del->pRight : del->pLeft;
		}
		if (!(del->data == key)) return false;  // ã�� �� ����. 

		// ��� 1, 2 ���� 
		delete del;
		
		// sib ã�� 
		if (delp != m_pNodeHead && bits(key, b-1, 1))
		{
			delp->pRight = m_pNodeTail;
			sib = delp->pLeft;
		}
		else
		{
			delp->pLeft = m_pNodeTail;
			sib = delp->pRight;
		}
		
		if (!IsBranch(sib))	// sib�� ��������̸� ��� 2
		{
			delp = cp;
			del = bits(key, cpb, 1) ? delp->pRight : delp->pLeft;
			b = cpb + 1;
			while (IsBranch(del))
			{
				delp = del;
				del = bits(key, b++, 1) ? delp->pRight : delp->pLeft;
				delete delp;
			}
			if (cp != m_pNodeHead && bits(key, cpb, 1)) cp->pRight = sib;
			else cp->pLeft = sib;
		}
		m_nCount--;
		return true;
	}
};

#endif