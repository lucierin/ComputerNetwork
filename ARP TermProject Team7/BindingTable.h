#pragma once
#include <vector>

#define SVector std::vector
#define TBindingTable(x) template<typename T> x BindingTable<T>

template <typename T>
class BindingTable
{
protected:
	SVector<T> data;
	CListBox* listbox;
public:
	BindingTable(void);
	~BindingTable(void);
	void Init(CListBox* listbox) { this->listbox = listbox; }
	void Clear();
	void RemoveAt(int index);
	void PushBack(T a);
	void AllUpdate();
	// 리스트박스에 문자열을 추가한다
	virtual void AddString(T a) {}
};

