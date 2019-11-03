#include "jstring.h"
#include <stdio.h>

using namespace handy::action_note;

size_t jstring::len(){
  return length;
}

const char * jstring::c_str(){
  return value;
}

bool jstring::equals(jstring other){
  if(this->length == other.length){
    for(size_t idx = 0; idx < this->length; idx++){
      if(this->value[idx] != other.value[idx]){
        return false;
      }
    }
    return true;
  }else{
    return false;
  }
}

bool jstring::contains(jstring other){
  int start;
  int stop;
  return contains(other, start, stop);
}

bool jstring::contains(jstring other, int &start, int &stop){
  start = stop = -1;
  bool foundLock = false;
  size_t currIdx = 0;
  size_t otherIdx = 0;
  
  if(other.length > this->length){
    return false;
  }

  while(currIdx < this->length && !foundLock){
    if(this->value[currIdx] == other.value[otherIdx]){
      otherIdx++;
      if(start < 0){
        start = currIdx;
      }
      if(otherIdx == other.length){
        foundLock = true;
        stop = currIdx;
      }
    }else{
      otherIdx = 0;
      start = -1;
      if((currIdx + other.length) > this->length){
        return false;
      }
    }
    currIdx++;
  }

  return foundLock;
}

std::vector<jstring> jstring::split(jstring delim){
	jstring*remainder = new jstring(this->value);
  int start = -1;
  int stop = -1;
  std::vector<jstring> retVal;
  while(remainder->contains(delim, start, stop)){
    char * buff = new char[start + 1];
    strncpy(buff, remainder->value, start);
    buff[start] = '\0';
    retVal.push_back(jstring(buff));
    delete[] buff;
    buff = new char[remainder->length - stop];
    const char *rP = remainder->value;
    strncpy(buff, (const char *) rP + stop + 1, remainder->length - (stop + 1));
    buff[remainder->length - (stop + 1)] = '\0';
	jstring*tmp = new jstring(buff);
    delete remainder;
    delete[] buff;
    remainder = tmp;
  }

  if(remainder->len() > 0){
    retVal.push_back(*remainder);
  }

  delete remainder;

  return retVal;
}

bool jstring::empty(){
  return this->length == 0;
}
