#ifndef _BTreeMap_h
#define _BTreeMap_h

namespace cppalgo
{
	template <class TYPE> class BTreeMap
	{
	public:
		enum Exception {
			INVALID_DIMENSION,		// BTree�� ������ 3�̻��� Ȧ������ �Ѵ�. 
		};
	public:
		BTreeMap(int dim = 5);
		~BTreeMap();
		// utilites
		long GetCount() const { return m_nCount; }
		bool IsEmpty() const { return m_nCount == 0; }
		void RemoveAll();
		// operations
		bool Find(const TYPE& key, TYPE& value) const;
		bool Insert(const TYPE& value);
		bool Remove(const TYPE& key);
		// BTree�� ���� �ߺ�Ű�� ���� ó���� �����. 
	protected:
		struct Node;
		typedef Node* PNODE;
		struct Node {
			TYPE *m_pKeys;
			Node **m_pChildren;
			int m_nKeys;

			Node(int dim) { 
				m_pKeys = new TYPE [dim];
				m_pChildren = new Node* [dim+ 1];
				for (int i = 0; i <= dim; i++) m_pChildren[i] = 0;
				m_nKeys = 0;
			}
			~Node() {
				delete [] m_pKeys;
				delete [] m_pChildren;
			}
			PNODE& Left(int n) { return m_pChildren[n]; }
			PNODE& Right(int n) { return m_pChildren[n+1]; }
			TYPE& Key(int n) { return m_pKeys[n]; }
			int& Size() { return m_nKeys; }
			void AddValue(const TYPE& value, Node* left, Node* right)
			{
				int i;
				i = m_nKeys;  // �������� 
				while (i > 0 && Key(i-1) > value)
				{
					Key(i) = Key(i-1);
					Left(i+1) = Left(i);
					i--;
				}
				m_nKeys++;
				Key(i) = value;
				Left(i) = left;
				Right(i) = right;
			}
			void DelValue(int index)
			{
				if (index < Size() - 1) {
					for (int i = index+1; i < Size(); i++)
					{
						Key(i-1) = Key(i);
						Left(i-1) = Left(i);
					}
					Left(i-1) = Left(i);
				}
				m_nKeys--;
			}
			int FindKey(const TYPE& key)
			{
				for (int i = 0; i < m_nKeys; i++)
					if (key == Key(i)) return i;
				return -1;
			}
			void Clear(int dim)
			{	
				m_nKeys = 0;
				for (int i = 0; i <= dim; i++)
					m_pChildren[i] = 0;
			}
		};
		Node* m_pNodeHead;
		void _RemoveSubtree(Node *pNode);
		Node* _Split(const TYPE& key, Node* pivot);
		bool _BorrowKey(Node *p, int index);
		Node* _BindNode(Node* p, int index);
		TYPE _SwapKey(Node *del, int index);

		long m_nCount;
		long m_nDim;
	};
	
	template <class TYPE>
		BTreeMap<TYPE>::BTreeMap(int dim)
	{
		if (dim < 3 || dim % 2 == 0)
			throw INVALID_DIMENSION;
			
		m_pNodeHead = new Node(dim);
		m_nDim = dim;
	}

	template <class TYPE>
		BTreeMap<TYPE>::~BTreeMap()
	{
		RemoveAll();
		delete m_pNodeHead;
	}

	template <class TYPE>
		void BTreeMap<TYPE>::_RemoveSubtree(Node *pNode)
	{
		if (pNode != 0)
		{
			for (int i = 0; i <= pNode->Size(); i++)
				_RemoveSubtree(pNode->Left(i));
			delete pNode;
		}
	}

	template <class TYPE>
		void BTreeMap<TYPE>::RemoveAll()
	{
		_RemoveSubtree(m_pNodeHead->Left(0));
		m_pNodeHead->Left(0) = 0;
	}

	template <class TYPE>
		bool BTreeMap<TYPE>::Find(const TYPE& key, TYPE& value) const
	{
		Node *t;
		int index;
		t = m_pNodeHead->Left(0);
		while (t != 0 && (index = t->FindKey(key)) < 0)
		{
			for (int i = 0; i < t->Size() && key > t->Key(i); i++);
			t = t->Left(i);
		}

		if (t == 0) return false;
		
		value = t->Key(index);
		return true;
	}

	template <class TYPE>
		BTreeMap<TYPE>::Node* BTreeMap<TYPE>::_Split(const TYPE& key, Node* pivot)
	{
		Node *left, *right;
		Node *split;   // ������ ��� 
		int i, j;
		
		right = new Node(m_nDim);

		// ��� 1 : ������ ��尡 Root �� ��� 
		if (pivot == m_pNodeHead)
		{
			split= pivot->Left(0);		// child == root

			// left child ���� 
			left = new Node(m_nDim);
			for (i = 0; i < m_nDim/2; i++)
			{
				left->Key(i) = split->Key(i);
				left->Left(i) = split->Left(i);
			}
			left->Left(i) = split->Left(i);
			left->Size() = m_nDim/2;

			// right child ���� 
			for (i = m_nDim/2+1, j = 0; i < m_nDim; i++, j++)
			{
				right->Key(j) = split->Key(i);
				right->Left(j) = split->Left(i);
			}
			right->Left(j) = split->Left(i);
			right->Size() = m_nDim/2;

			// �θ��� ���� 
			TYPE temp = split->Key(m_nDim/2);
			split->Clear(m_nDim);
			split->AddValue(temp, left, right);
		} 
		// ��� 2 : ������ ��尡 Root�� �ƴ� ��� 
		else
		{
			// ������ ��带 ã�� 
			for (i = 0; i < pivot->Size() && key > pivot->Key(i); i++);

			// ���� ���� �̹� �ִ� ����̹Ƿ� ������ ���� 
			left = pivot->Left(i);
			left->Size() = m_nDim/2;

			// ������ ��� ���� 
			for (i = m_nDim/2+1, j = 0; i < m_nDim; i++, j++)
			{
				right->Key(j) = left->Key(i);
				right->Left(j) = left->Left(i);
			}
			right->Left(j) = left->Left(i);
			right->Size() = m_nDim / 2;

			// �߰�Ű�� �θ� ���� 
			pivot->AddValue(left->Key(m_nDim/2), left, right);
			split = pivot;
		}
		return split;
	}

	template <class TYPE>
		bool BTreeMap<TYPE>::Insert(const TYPE& value)
	{
		Node *t, *p;
		int i;
		p = m_pNodeHead;
		t = m_pNodeHead->Left(0);
		if (t == 0)  // �Ѹ���尡 ���ٸ� Ư���� �������־�� �Ѵ�. 
		{
			t = new Node(m_nDim);
			m_pNodeHead->Left(0) = t;
		}
		while (t != 0)
		{
			if (t->FindKey(value) >= 0)		// �ߺ�Ű ���Ա��� 
				return false;
			if (t->Size() == m_nDim)	// ���� ����̸� �����Ѵ�. 
				t = _Split(value, p);
			p = t;
			for (i = 0; i < t->Size() && value > t->Key(i); i++);
			t = t->Left(i);
		}
		p->AddValue(value, 0, 0);   // �ܺγ��� ���� 
		m_nCount++;
		return true;
	}

	template <class TYPE>
		bool BTreeMap<TYPE>::_BorrowKey(Node *p, int index)
	{
		int from, to;
		Node *p1, *p2;
		to = index;
		if (index == p->Size())	// ���� �������� ��� ������������ ���� 
			from = index - 1;
		else  // �ƴϸ� ������ �������Լ� ���� 
			from = index + 1;
		p1 = p->Left(from);   // p1 = �����ִ� ��� 
		p2 = p->Left(to);       // p2 = ������ ��� 
		if (p1->Size() <= m_nDim/2)   // ������ Ű�� ���� �� ���и� ���� 
			return false;
		if (from > to)   // ������ �������Լ� ���� 
		{
			p2->AddValue(p->Key(to), p2->Left(p2->Size()), p1->Left(0));
			p->Key(to) = p1->Key(0);
			p1->DelValue(0);
		}
		else   // ���� �������Լ� ���� 
		{
			p2->AddValue(p->Key(from), p1->Left(p1->Size()), p2->Left(0));
			p->Key(from) = p1->Key(p1->Size() - 1);
			p1->DelValue(p1->Size() - 1);
		}
		return true;
	}

	template <class TYPE>
		BTreeMap<TYPE>::Node* BTreeMap<TYPE>::_BindNode(Node* p, int index)
	{
		Node *left, *right;
		int i;
		if (index == p->Size()) index--;	// ���� �������̸� index ���� 
		left = p->Left(index);
		right = p->Right(index);
		left->Key(left->Size()++) = p->Key(index);   // ���ʳ�忡 �θ�Ű�� ���� 
		for (i = 0; i < right->Size(); i++)  // ���ʳ�忡 ������ ��带 ���� 
		{
			left->Key(left->Size()) = right->Key(i);
			left->Left(left->Size()++) = right->Left(i);
		}
		left->Left(left->Size()) = right->Left(i);
		p->DelValue(index);   // �θ��忡�� ������ Ű�� ���� 
		p->Left(index) = left;  // ������ ���� 
		delete right;
		if (p->Size() == 0)   // �Ѹ������ �� �ۿ� ���� �̰��....
		{
			delete p;
			m_pNodeHead->Left(0) = left;
		}
		return left;  // ���յ� ��带 ���� 
	}

	template <class TYPE>
		TYPE BTreeMap<TYPE>::_SwapKey(Node *del, int index)
	{
		Node *cdd, *cddp;    // cdd�� ��ü���, cddp�� ��ü����� �θ� 
		cddp = del;
		cdd = cddp->Right(index);   // ����Ű�� ������ �ڽ� 
		while (cdd->Left(0) != 0)
		{
			cddp = cdd;
			cdd = cdd->Left(0);
		}
		del->Key(index) = cdd->Key(0);   // Ű ��ü 
		return cdd->Key(0);
	}

	template <class TYPE>
		bool BTreeMap<TYPE>::Remove(const TYPE& key)
	{
		Node *t, *p;
		int pi = 0;   // �θ��� index
		int ti = 0;   // �������� index
		TYPE value = key;
		
		p = m_pNodeHead;
		t = m_pNodeHead->Left(0);
		while (t != 0)
		{
			if (t->Size() <= m_nDim/2 && p != m_pNodeHead) // Ȯ���� �ʿ䰡 ������ Ȯ�� 
			{
				if (!_BorrowKey(p, pi))		// �������Լ� �������� �����ϸ� ������ ���� 
					t = _BindNode(p, pi);
			}

			if ((ti = t->FindKey(value)) >= 0)  // ����Ű�� �� ��忡 ������ 
			{
				if (t->Left(0) == 0) break;  // �ܺγ���̸� break;
				else value = _SwapKey(t, ti);   // ���γ���̸� �ٲ�. ���� ���ο� value�� �Ʒ��� ������ �Ѵ�. 
			}
			p = t;
			for (pi = 0; pi < t->Size() && (value > t->Key(pi) || value == t->Key(pi)); pi++);
			t = t->Left(pi);
		}
		if (t == 0) return false;   // ã�� �� ����. 
		if (t->Size() <= m_nDim/2 && p != m_pNodeHead)  // �ܺγ���ε� Ű���� �ʹ� ������ 
			if (!_BorrowKey(p, pi)) t = _BindNode(p, pi);
		t->DelValue(t->FindKey(value));  // ����� Ű�� ���� 
		m_nCount--;
		return true;
	}
};

#endif