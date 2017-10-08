/*
  This file is part of TS software suite.

  TS software suite is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  TS software suite is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with TS software suite.  If not, see <http://www.gnu.org/licenses/>.

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
