#include "StdAfx.h"
#include "BindingTable.h"


TBindingTable()::BindingTable(void)
{
	listbox = 0;
}


TBindingTable()::~BindingTable(void)
{
}

TBindingTable(void)::Clear()
{
	data.clear();
	listbox->ResetContent();
}

TBindingTable(void)::RemoveAt(int index)
{
	int i = 0;
	for(SVector<T>::iterator i = data.begin(); i != data.end();)
	{
		if(i == index)
		{
			T temp = (*i);
			i = data.erase(i);
			delete temp;
			break;
		}
	}
	listbox->DeleteString(index);
}

TBindingTable(void)::PushBack(T a)
{
	data.push_back(a);
	AddString(a);
}

TBindingTable(void)::AllUpdate()
{
	listbox->ResetContent();
	for(int i = 0; i < data.size(); i++)
		AddString(data[i]);
}