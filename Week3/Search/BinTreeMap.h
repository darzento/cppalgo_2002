#ifndef _BinTreeMap_h
#define _BinTreeMap_h

#include "../Tree/BinaryTree.h"

namespace cppalgo
{
	template <class TYPE> class BinTreeMap : public BinaryTree<TYPE>
	{
	public:
		enum Exception {
			INSUFFICIENT_MEMORY_FOR_SORT
		};
	public:
		BinTreeMap() : BinaryTree<TYPE>() { m_nCount = 0; }
		~BinTreeMap() {}        

		// Utilities
		long GetCount() const { return m_nCount; }
		bool IsEmpty() const { return m_nCount == 0; }
		void RemoveAll() { BinaryTree<TYPE>::RemoveAll(); m_nCount = 0; }

		// Operations
		bool Find(const TYPE& key, TYPE& value) const;
		bool Insert(const TYPE& value);
		bool Remove(const TYPE& key);

		// FindFirst/FindNext�� ���� ���� : ���� ����Ÿ��� ������ ����.
		// RemoveAt�� ���� ���� : �θ��带 ���� ����. 
		// �׷��� BinaryTree�� �ߺ�Ű�� �ٷ�� �����. 

		// �迭 a�� ������ ũ�Ⱑ ���ƾ� �Ѵ�. 
		void Sort(TYPE a[], int n, Node* p = 0) const;
		void Balance();

	protected:
		long m_nCount;
		
		Node* _balance(TYPE a[], int n, bool start = true);
	};

	template <class TYPE>
		bool BinTreeMap<TYPE>::Find(const TYPE& key, TYPE& value) const
	{
		Node *s;

		s = m_pNodeHead->pLeft;
		while (!(key == s->data) && s != m_pNodeTail)
		{
			if (key > s->data) s = s->pRight;
			else s = s->pLeft;
		}
		if (s == m_pNodeTail)
			return false;

		value = s->data;
		return true;
	}

	template <class TYPE>
		bool BinTreeMap<TYPE>::Insert(const TYPE& value)
	{
		Node *p, *s;
		p = m_pNodeHead;
		s = m_pNodeHead->pLeft;

		while (s != m_pNodeTail)
		{
			p = s;   // �θ� �����Ѵ�.
			if (value > s->data) s = s->pRight;
			else s = s->pLeft;
		}

		s = new Node;
		s->data = value;
		s->pLeft = m_pNodeTail;
		s->pRight = m_pNodeTail;

		if (value > p->data && p != m_pNodeHead) p->pRight = s;
		else p->pLeft = s;

		m_nCount++;
		return true;
	}

	template <class TYPE>
		bool BinTreeMap<TYPE>::Remove(const TYPE& key)
	{
		if (IsEmpty()) return false;
		Node *delp, *del;
		Node *cdd, *cddp;

		// ���� ������ Ű�� �ִ� ��带 ã�´�. 
		delp = m_pNodeHead;
		del = m_pNodeHead->pLeft;
		while (!(key == del->data) && del != m_pNodeTail)
		{
			delp = del;
			if (key > del->data) del = del->pRight;
			else del = del->pLeft;
		}
		if (del == m_pNodeTail)	// �����ϰ��� �ϴ� Ű�� ���� 
			return false;

		if (del->pRight == m_pNodeTail)  // ��� 1
		{
			cdd = del->pLeft;
		}
		else if (del->pRight->pLeft == m_pNodeTail) // ��� 2
		{
			cdd = del->pRight;
			cdd->pLeft = del->pLeft;
		}
		else		// ��� 3
		{
			cddp = del;
			cdd = del->pRight;
			while (cdd->pLeft != m_pNodeTail)
			{
				cddp = cdd;
				cdd = cdd->pLeft;			
			}

			cddp->pLeft = cdd->pRight;
			cdd->pLeft = del->pLeft;
			cdd->pRight = del->pRight;
		}

		// delp�� �ڽ����� cdd�� �� 
		if (key > delp->data && delp != m_pNodeHead) delp->pRight = cdd;
		else delp->pLeft = cdd;

		delete del;
		m_nCount--;
		return true;
	}

	template <class TYPE>
		void BinTreeMap<TYPE>::Sort(TYPE a[], int n, Node* p) const
	{
		static int index = 0;
		if (p == 0)		// �ʱ����� 
		{
			if (m_nCount != n) throw INSUFFICIENT_MEMORY_FOR_SORT;
			index = 0;
			p = m_pNodeHead->pLeft;
		}
		
		if (p != m_pNodeTail)  // ���� ���� 
		{
			Sort(a, n, p->pLeft);
			a[index++] = p->data;
			Sort(a, n, p->pRight);
		}
	}

	template <class TYPE>
		void BinTreeMap<TYPE>::Balance()
	{
		TYPE *a;
		a = new TYPE [m_nCount];
		Sort(a, m_nCount);

		int nCount = m_nCount;
		RemoveAll();

		m_pNodeHead->pLeft = _balance(a, nCount, true);
		m_nCount = nCount;

		delete [] a;
	}

	template <class TYPE>
		BinTreeMap<TYPE>::Node* BinTreeMap<TYPE>::_balance(TYPE a[], int n, bool start)
	{
		static int index = 0;
		if (start)
			index = 0;
		
		Node *p;
		int nl, nr;
		if (n > 0)
		{
			nl = (n-1) / 2;
			nr = n - nl - 1;
			
			p = new Node;
			p->pLeft = _balance(a, nl, false);
			p->data = a[index++];
			p->pRight = _balance(a, nr, false);

			return p;
		}
		else
			return m_pNodeTail;
	}
};

#endif