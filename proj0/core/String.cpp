// String.cpp
//
// ICS 46 Spring 2020
// Project #0: Getting to Know the ICS 46 VM
//
// Implement all of your String member functions in this file.
//
// Note that the entire standard library -- both the C Standard
// Library and the C++ Standard Library -- is off-limits for this
// task, as the goal is to exercise your low-level implementation
// skills (pointers, memory management, and so on).

#include "String.hpp"
#include "OutOfBoundsException.hpp"
String::String()
{
    len = 0;
	myString = new char[1];
    myString[0] = '\0';
}
String::String(const char* chars)
{
    int count = 0;
	while(chars[count]!='\0')
	{
		count++;
	}
	myString = new char[count+1];
	for(unsigned int i=0; i<count; i++)
	{
		myString[i] = chars[i];
	}
	len = count;
	myString[len] = '\0';
}
String::String(const String& s)
{
    len = s.length();
    myString = new char[len+1];
    for(unsigned int i=0; i<len; i++)
    {
        myString[i] = s.myString[i];

    }
        myString[len]='\0';
}

String::~String() noexcept
{
    delete[] myString;
}
String& String::operator=(const String& s)
{
    delete[] myString;
    len = s.length();
    myString = new char[len+1];
    for(int i = 0; i < len; i++)
    {
        myString[i] = s.myString[i];
    }
    myString[len] = '\0';
    return *this;
}

void String::append(const String& s)
{

  int len_1 = length();
  int len_2 = s.length();
  char* temp = new char[len_1+1];
  for(unsigned int i=0; i<length();i++)
  {
      temp[i] = myString[i];
  }
  temp[len_1] = '\0';
  int total_len = len_1 + len_2;
  myString = new char[total_len+1];
  for(unsigned int j=0; j<len_1; j++)
  {
      myString[j] = temp[j];
  }
  for(unsigned int j=0; j<len_2; j++)
  {
      myString[len_1+j] = s.myString[j];
  }
  myString[total_len] = '\0';
  delete[] temp;
}
char String::at(unsigned int index) const
{
    if(index >= length() || index <0)
    {
        throw OutOfBoundsException();
    }else
    {
        return myString[index];
    }
}
char& String::at(unsigned int index)
{
    if(index >= length() || index <0)
    {
        throw OutOfBoundsException();
    }else
    {
        return myString[index];
    }
}
void String::clear(){
    delete[] myString;
    myString = new char[1];
    myString[0] = '\0';
}
int String::compareTo(const String& s) const noexcept
{
     if(length() < s.length() ){
         return 1;
     }
     else if(length() > s.length()){
         return -1;
     }
     else{
         return 0;
     }
}
String String::concatenate(const String& s) const
{
    int len_1 = length();
	int len_2 = s.length();
	char* temp = new char[len_1+len_2+1];
	for(unsigned int i = 0;i<len_1;i++)
	{
		temp[i] = myString[i];
	}
	for(unsigned int j = 0;j<len_2;j++)
	{
		temp[len_1+j] = s.myString[j];
	}
	temp[len_1+len_2] = '\0';
	return temp;
}

bool String::contains(const String& substring) const noexcept
{
    int sub_len = substring.length();
    int str_len = length();
    if(sub_len > str_len)
    {
        return false;
    }
    for(unsigned int i=0; i<str_len; i++)
    {
        int j=0;
        if(myString[i] == substring.myString[j])
        {
            int k = i;
            while(myString[i] == substring.myString[j] && j< sub_len)
            {
                j++;
                i++;
            }
            if(j == sub_len)
                {return true;}
            else
               { i=k;}
        }
    }
    return false;
}
bool String::equals(const String& s) const noexcept
{
     if(length() == s.length())
	{
		int count=0;
		for(unsigned int i=0; i<length();i++)
		{
			if(myString[i]==s.myString[i])
				count++;
		}
		if(count==length())
		{
			return true;
		}
		else
		{
		    return false;
		}
	}else
	{
		return false;
	}
}
int String::find(const String& substring) const noexcept
{
    int sub_len = substring.length();
    int str_len = length();
    if(sub_len > str_len)
    {
        return -1;
    }
    for(unsigned int i=0; i<str_len; i++)
    {
        int j=0;
        if(myString[i] == substring.myString[j])
        {
            int k = i;
        while(myString[i] == substring.myString[j] && j< sub_len)
            {
                j++;
                i++;
            }
            if(j == sub_len)
                return (i-j);
            else
                i=k;
        }
    }
    return -1;
}
bool String::isEmpty() const noexcept
{
    if(myString[0] == '\0')
    {
        return true;
    }
    else
    {
        return false;
    }
}
String String::substring(unsigned int startIndex, unsigned int endIndex) const
{
    if(startIndex >= length() || endIndex >= length() 
        || startIndex <0 || endIndex <0)
	{
		throw OutOfBoundsException();
	}
	else
	{
		char* temp = new char[endIndex-startIndex+1];
		int j = 0;
		for(unsigned int i = startIndex; i<endIndex; i++)
		{
			temp[j] = myString[i];
			j++;
		}
        temp[endIndex-startIndex] = '\0';
        return temp;
	}
}
const char* String::toChars() const noexcept
{
    return myString;
}

unsigned int String::length() const noexcept
{
    int i = 0;
    while (myString[i] != '\0')
    {
        i++;
    }
    return i;
}


