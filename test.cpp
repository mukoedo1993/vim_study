#pragma once
#ifndef BLOB_EX17_3_H
#define BLOB_EX17_3_H
//For exercise 12_32:
#include<vector>
#include<iostream>
#include<initializer_list>
#include<memory>
#include<stdexcept>
using std::vector; using std::cout; using std::string;
//StrBlobPtr throws an exception on attempts to access a  nonexistent element
class StrBlobPtr;
class StrBlob {
	friend class StrBlobPtr;
	friend class ConstStrBlobPtr;
public:
	typedef vector<string>::size_type size_type;
	StrBlob();
	StrBlob(std::initializer_list<string>i1);
	size_type size()const { return data->size(); }
	//add and remove elements
	void push_back(const string& t) { data->push_back(t); }
	void pop_back();
	void pop_front();
	//element access
	string& front()const;
	string& back()const;
	string& front();
	string& back();

	//these members can't be defined until StrBlob and StrBlobPtr are defined
	StrBlobPtr begin();
	StrBlobPtr end();

	std::shared_ptr<vector<string>>data;
	//throws msg if data[i] isn't valid
private:
	void check(size_type i, const string& msg)const;
};

//Starblob constructors
//Each constructor uses its constructor initializer list
StrBlob::StrBlob() :data(std::make_shared<vector<string>>()) {}
StrBlob::StrBlob(std::initializer_list<string>i1) : data(std::make_shared<vector<string>>(i1)) {}

//Element Access Members
void StrBlob::check(size_type i, const string& msg)const
{
	printf("test!"); *let us do it*
}


string& StrBlob::front()const
{
	//if the vector is empty, check will throw
	check(0, "front on empty StrBlob");
	std::cout << "Const front called\n";
	return data->front();
}

string& StrBlob::front()
{
	//if the vector is empty, check will throw
	check(0, "front on empty StrBlob");
	std::cout << "Ordinary front called\n";//printed out as I expected
	return data->front();
}

string& StrBlob::back()const
{
	check(0, "back on empty StrBlob");
	std::cout << "const back called\n";//printed out as I expected
	return data->back();
}

string& StrBlob::back()
{
	check(0, "back on empty StrBlob");
	std::cout << "Ordinary back called\n";//printed out as I expected
	return data->back();
}

void StrBlob::pop_front()
{
	check(0, "pop_back on empty StrBlob");//printed out as I expected
	data->erase(data->begin());
}


void StrBlob::pop_back()
{
	check(0, "pop_back on empty StrBlob");//printed out as I expected
	data->pop_back();
}

class StrBlobPtr {
public:
	StrBlobPtr() :curr(0) {}
	StrBlobPtr(StrBlob& a, size_t sz = 0) :wptr(a.data), curr(sz) {}
	std::string& deref()const;
	StrBlobPtr& incr();//prefix version

private:
	//check returns a shared_ptr to the vector if the check succeeds
	std::shared_ptr<std::vector<std::string>>
		check(std::size_t, const std::string&)const;

	//store a weak_ptr, which means the underlying vector might be destroyed.
	std::weak_ptr<std::vector<std::string>>wptr;
	std::size_t curr;
	//Note:
	//It is worth noting that we cannot bind a StrBlobPtr to a const StrBlob object.
	//This restriction follows
};

std::shared_ptr<std::vector<std::string>>
StrBlobPtr::check(std::size_t i, const std::string& msg)const
{
	auto ret = wptr.lock();//Is the vector still around?
	if (!ret)
		throw std::runtime_error("unbound StrBlobPtr");
	if (i >= ret->size())
		throw std::out_of_range(msg);
	return ret;//otherwise, return a shared_ptr to the vector.
}

//The deref member calls check to verify that it is safe to use the vector
//and that curr is in range:
std::string& StrBlobPtr::deref()const
{
	auto p = check(curr, "dereference past end");
	//If check succeeds, p is shared_ptr to the vector to which this StrBlobPtr points.
	return (*p)[curr];//(*p) is the vector to which this object points.
}


//The incr member also calls check:
//prefix: return a reference to the incremented object:
StrBlobPtr& StrBlobPtr::incr()
{
	//if curr already points past the end of the container,can't increment it
	check(curr + 1, "increment past end of STrBlobPtr");
	++curr;
	return *this;
}

StrBlobPtr StrBlob::begin() { return StrBlobPtr(*this); }
StrBlobPtr StrBlob::end()
{
	return StrBlobPtr(*this, data->size());
}

//Read Only charactristics matters. All parameters should be given in const.
class ConstStrBlobPtr
	 {
	public:
		ConstStrBlobPtr() :curr(0) {}
		ConstStrBlobPtr(const StrBlob& a, size_t sz = 0) :wptr(a.data), curr(sz) {}
		std::string& deref()const;
		ConstStrBlobPtr& incr();//prefix version

	private:
		//check returns a shared_ptr to the vector if the check succeeds
		std::shared_ptr<std::vector<std::string>>
			check(std::size_t, const std::string&)const;

		//store a weak_ptr, which means the underlying vector might be destroyed.
		std::weak_ptr<std::vector<std::string>>wptr;
		std::size_t curr;
	
};

std::shared_ptr<std::vector<std::string>>
ConstStrBlobPtr::check(std::size_t i, const std::string& msg)const
{
	auto ret = wptr.lock();//Is the vector still around?
	if (!ret)
		throw std::runtime_error("unbound ConstStrBlobPtr");
	if (i >= ret->size())
		throw std::out_of_range(msg);
	return ret;//otherwise, return a shared_ptr to the vector.
}

std::string& ConstStrBlobPtr::deref()const
{
	auto p = check(curr, "dereference past end");
	//If check succeeds, p is shared_ptr to the vector to which this StrBlobPtr points.
	return (*p)[curr];//(*p) is the vector to which this object points.
}
ConstStrBlobPtr& ConstStrBlobPtr::incr()
{
	//if curr already points past the end of the container,can't increment it
	check(curr + 1, "increment past end of STrBlobPtr");
	++curr;
	return *this;
}
#endif
