// This may look like C code, but it is really -*- C++ -*-
/*
 * 
 * 
 * @(#)$Id$
 */
#ifndef	SPOOL_H
#define	SPOOL_H
#ifdef IMPLEMENTING_SPOOL
static const char Spool_rcsid[] = "@(#)$Id$";
#endif
#include <assert.h>

template<class T> class Spool {
public:
   Spool(int size);
   virtual ~Spool();
   int append(const T &data);
   void flush();
   int length() const {
      return len;
   }
   const T *dataAt(int idx) const;
private:
   T **datav;
   int len;
   int size;
};

template<class T> Spool<T>::Spool(int sz) {
   size = sz;
   datav = new T*[size];
   assert(datav != NULL);
   len = 0;
}

template<class T> Spool<T>::~Spool() {
   flush();
   delete [] datav;
}

template<class T> int Spool<T>::append(const T &data) {
   if (len == size) return -1;
   datav[len] = new T(data);
   len++;
   return 0;
}

template<class T> void Spool<T>::flush() {
   for (int i = 0; i < len; i++) {
      delete datav[i];
   }
   len = 0;
}

template<class T> const T *Spool<T>::dataAt(int idx) const {
   assert(idx < len);
   return datav[idx];
}

#endif	/* SPOOL_H */
