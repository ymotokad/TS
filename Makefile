CXXFLAGS=-g -DNDEBUG
CXXFLAGS=-g
LIBS=

OBJS=StdLogger.o \
	ByteArray.o ByteArrayBuffer.o BitStream.o \
	Table.o \
	ProgramAssociationSection.o ProgramMapSection.o \
	AdaptationField.o Descriptors.o \
	MJD.o \
	ServiceDescriptionTable.o EventInformationTable.o \
	TimeDateSection.o \
	TransportPacket.o TransportStream.o

all: ts_dump

ts_dump: $(OBJS) ts_dump.o
	$(CXX) -o $@ $(OBJS) ts_dump.o $(LIBS)

clean:
	@rm -f $(OBJS) *.o *~ ts_dump

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

depend:
	sed '/^# DO NOT DELETE THIS LINE/,$$d'<Makefile>Makefile.tmp
	echo '# DO NOT DELETE THIS LINE' >>Makefile.tmp
	$(CXX) -M $(CXXFLAGS) *.cc >>Makefile.tmp
	rm -f Makefile.bak
	mv Makefile Makefile.bak
	mv -f Makefile.tmp Makefile


# DO NOT DELETE THIS LINE
AdaptationField.o: AdaptationField.cc /usr/include/assert.h \
  /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/stdlib.h \
  /usr/include/Availability.h /usr/include/AvailabilityInternal.h \
  /usr/include/_types.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/i386/_types.h \
  /usr/include/sys/wait.h /usr/include/sys/signal.h \
  /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h \
  /usr/include/i386/signal.h /usr/include/i386/_structs.h \
  /usr/include/sys/_structs.h /usr/include/machine/_structs.h \
  /usr/include/mach/i386/_structs.h /usr/include/sys/resource.h \
  /usr/include/machine/endian.h /usr/include/i386/endian.h \
  /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/i386/_types.h /usr/include/c++/4.2.1/iostream \
  /usr/include/c++/4.2.1/bits/c++config.h \
  /usr/include/c++/4.2.1/bits/os_defines.h /usr/include/unistd.h \
  /usr/include/sys/unistd.h /usr/include/sys/select.h \
  /usr/include/sys/_select.h /usr/include/c++/4.2.1/bits/cpu_defines.h \
  /usr/include/c++/4.2.1/ostream /usr/include/c++/4.2.1/ios \
  /usr/include/c++/4.2.1/iosfwd /usr/include/c++/4.2.1/bits/c++locale.h \
  /usr/include/c++/4.2.1/clocale /usr/include/locale.h \
  /usr/include/_locale.h /usr/include/c++/4.2.1/cstring \
  /usr/include/c++/4.2.1/cstddef \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stddef.h \
  /usr/include/string.h /usr/include/strings.h \
  /usr/include/c++/4.2.1/cstdio /usr/include/stdio.h \
  /usr/include/c++/4.2.1/cstdarg \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  /usr/include/c++/4.2.1/bits/c++io.h /usr/include/c++/4.2.1/bits/gthr.h \
  /usr/include/c++/4.2.1/bits/gthr-default.h /usr/include/pthread.h \
  /usr/include/pthread_impl.h /usr/include/sched.h /usr/include/time.h \
  /usr/include/_structs.h /usr/include/c++/4.2.1/cctype \
  /usr/include/ctype.h /usr/include/runetype.h \
  /usr/include/c++/4.2.1/bits/stringfwd.h \
  /usr/include/c++/4.2.1/bits/postypes.h /usr/include/c++/4.2.1/cwchar \
  /usr/include/c++/4.2.1/ctime /usr/include/wchar.h \
  /usr/include/_wctype.h /usr/include/stdint.h \
  /usr/include/c++/4.2.1/bits/functexcept.h \
  /usr/include/c++/4.2.1/exception_defines.h \
  /usr/include/c++/4.2.1/exception \
  /usr/include/c++/4.2.1/bits/char_traits.h \
  /usr/include/c++/4.2.1/bits/stl_algobase.h \
  /usr/include/c++/4.2.1/climits \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /usr/include/limits.h /usr/include/machine/limits.h \
  /usr/include/i386/limits.h /usr/include/i386/_limits.h \
  /usr/include/sys/syslimits.h /usr/include/c++/4.2.1/cstdlib \
  /usr/include/c++/4.2.1/bits/stl_pair.h \
  /usr/include/c++/4.2.1/bits/cpp_type_traits.h \
  /usr/include/c++/4.2.1/ext/type_traits.h /usr/include/c++/4.2.1/utility \
  /usr/include/c++/4.2.1/bits/stl_relops.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.2.1/bits/concept_check.h \
  /usr/include/c++/4.2.1/bits/stl_iterator.h \
  /usr/include/c++/4.2.1/debug/debug.h \
  /usr/include/c++/4.2.1/bits/localefwd.h \
  /usr/include/c++/4.2.1/bits/ios_base.h \
  /usr/include/c++/4.2.1/ext/atomicity.h \
  /usr/include/c++/4.2.1/bits/atomic_word.h \
  /usr/include/c++/4.2.1/bits/locale_classes.h \
  /usr/include/c++/4.2.1/string /usr/include/c++/4.2.1/memory \
  /usr/include/c++/4.2.1/bits/allocator.h \
  /usr/include/c++/4.2.1/bits/c++allocator.h \
  /usr/include/c++/4.2.1/ext/new_allocator.h /usr/include/c++/4.2.1/new \
  /usr/include/c++/4.2.1/bits/stl_construct.h \
  /usr/include/c++/4.2.1/bits/stl_uninitialized.h \
  /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h \
  /usr/include/c++/4.2.1/limits \
  /usr/include/c++/4.2.1/bits/ostream_insert.h \
  /usr/include/c++/4.2.1/bits/stl_function.h \
  /usr/include/c++/4.2.1/bits/basic_string.h \
  /usr/include/c++/4.2.1/algorithm /usr/include/c++/4.2.1/bits/stl_algo.h \
  /usr/include/c++/4.2.1/bits/stl_heap.h \
  /usr/include/c++/4.2.1/bits/stl_tempbuf.h \
  /usr/include/c++/4.2.1/bits/basic_string.tcc \
  /usr/include/c++/4.2.1/streambuf \
  /usr/include/c++/4.2.1/bits/streambuf.tcc \
  /usr/include/c++/4.2.1/bits/basic_ios.h \
  /usr/include/c++/4.2.1/bits/streambuf_iterator.h \
  /usr/include/c++/4.2.1/bits/locale_facets.h \
  /usr/include/c++/4.2.1/cwctype /usr/include/wctype.h \
  /usr/include/c++/4.2.1/bits/ctype_base.h \
  /usr/include/c++/4.2.1/bits/ctype_inline.h \
  /usr/include/c++/4.2.1/bits/codecvt.h \
  /usr/include/c++/4.2.1/bits/time_members.h \
  /usr/include/c++/4.2.1/bits/messages_members.h \
  /usr/include/c++/4.2.1/bits/basic_ios.tcc \
  /usr/include/c++/4.2.1/bits/ostream.tcc /usr/include/c++/4.2.1/locale \
  /usr/include/c++/4.2.1/bits/locale_facets.tcc \
  /usr/include/c++/4.2.1/typeinfo /usr/include/c++/4.2.1/istream \
  /usr/include/c++/4.2.1/bits/istream.tcc AdaptationField.h BitStream.h \
  TSTypes.h Logger.h ByteArrayBuffer.h ByteArray.h
BitStream.o: BitStream.cc /usr/include/assert.h /usr/include/sys/cdefs.h \
  /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/stdlib.h \
  /usr/include/Availability.h /usr/include/AvailabilityInternal.h \
  /usr/include/_types.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/i386/_types.h \
  /usr/include/sys/wait.h /usr/include/sys/signal.h \
  /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h \
  /usr/include/i386/signal.h /usr/include/i386/_structs.h \
  /usr/include/sys/_structs.h /usr/include/machine/_structs.h \
  /usr/include/mach/i386/_structs.h /usr/include/sys/resource.h \
  /usr/include/machine/endian.h /usr/include/i386/endian.h \
  /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/i386/_types.h BitStream.h /usr/include/c++/4.2.1/iostream \
  /usr/include/c++/4.2.1/bits/c++config.h \
  /usr/include/c++/4.2.1/bits/os_defines.h /usr/include/unistd.h \
  /usr/include/sys/unistd.h /usr/include/sys/select.h \
  /usr/include/sys/_select.h /usr/include/c++/4.2.1/bits/cpu_defines.h \
  /usr/include/c++/4.2.1/ostream /usr/include/c++/4.2.1/ios \
  /usr/include/c++/4.2.1/iosfwd /usr/include/c++/4.2.1/bits/c++locale.h \
  /usr/include/c++/4.2.1/clocale /usr/include/locale.h \
  /usr/include/_locale.h /usr/include/c++/4.2.1/cstring \
  /usr/include/c++/4.2.1/cstddef \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stddef.h \
  /usr/include/string.h /usr/include/strings.h \
  /usr/include/c++/4.2.1/cstdio /usr/include/stdio.h \
  /usr/include/c++/4.2.1/cstdarg \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  /usr/include/c++/4.2.1/bits/c++io.h /usr/include/c++/4.2.1/bits/gthr.h \
  /usr/include/c++/4.2.1/bits/gthr-default.h /usr/include/pthread.h \
  /usr/include/pthread_impl.h /usr/include/sched.h /usr/include/time.h \
  /usr/include/_structs.h /usr/include/c++/4.2.1/cctype \
  /usr/include/ctype.h /usr/include/runetype.h \
  /usr/include/c++/4.2.1/bits/stringfwd.h \
  /usr/include/c++/4.2.1/bits/postypes.h /usr/include/c++/4.2.1/cwchar \
  /usr/include/c++/4.2.1/ctime /usr/include/wchar.h \
  /usr/include/_wctype.h /usr/include/stdint.h \
  /usr/include/c++/4.2.1/bits/functexcept.h \
  /usr/include/c++/4.2.1/exception_defines.h \
  /usr/include/c++/4.2.1/exception \
  /usr/include/c++/4.2.1/bits/char_traits.h \
  /usr/include/c++/4.2.1/bits/stl_algobase.h \
  /usr/include/c++/4.2.1/climits \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /usr/include/limits.h /usr/include/machine/limits.h \
  /usr/include/i386/limits.h /usr/include/i386/_limits.h \
  /usr/include/sys/syslimits.h /usr/include/c++/4.2.1/cstdlib \
  /usr/include/c++/4.2.1/bits/stl_pair.h \
  /usr/include/c++/4.2.1/bits/cpp_type_traits.h \
  /usr/include/c++/4.2.1/ext/type_traits.h /usr/include/c++/4.2.1/utility \
  /usr/include/c++/4.2.1/bits/stl_relops.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.2.1/bits/concept_check.h \
  /usr/include/c++/4.2.1/bits/stl_iterator.h \
  /usr/include/c++/4.2.1/debug/debug.h \
  /usr/include/c++/4.2.1/bits/localefwd.h \
  /usr/include/c++/4.2.1/bits/ios_base.h \
  /usr/include/c++/4.2.1/ext/atomicity.h \
  /usr/include/c++/4.2.1/bits/atomic_word.h \
  /usr/include/c++/4.2.1/bits/locale_classes.h \
  /usr/include/c++/4.2.1/string /usr/include/c++/4.2.1/memory \
  /usr/include/c++/4.2.1/bits/allocator.h \
  /usr/include/c++/4.2.1/bits/c++allocator.h \
  /usr/include/c++/4.2.1/ext/new_allocator.h /usr/include/c++/4.2.1/new \
  /usr/include/c++/4.2.1/bits/stl_construct.h \
  /usr/include/c++/4.2.1/bits/stl_uninitialized.h \
  /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h \
  /usr/include/c++/4.2.1/limits \
  /usr/include/c++/4.2.1/bits/ostream_insert.h \
  /usr/include/c++/4.2.1/bits/stl_function.h \
  /usr/include/c++/4.2.1/bits/basic_string.h \
  /usr/include/c++/4.2.1/algorithm /usr/include/c++/4.2.1/bits/stl_algo.h \
  /usr/include/c++/4.2.1/bits/stl_heap.h \
  /usr/include/c++/4.2.1/bits/stl_tempbuf.h \
  /usr/include/c++/4.2.1/bits/basic_string.tcc \
  /usr/include/c++/4.2.1/streambuf \
  /usr/include/c++/4.2.1/bits/streambuf.tcc \
  /usr/include/c++/4.2.1/bits/basic_ios.h \
  /usr/include/c++/4.2.1/bits/streambuf_iterator.h \
  /usr/include/c++/4.2.1/bits/locale_facets.h \
  /usr/include/c++/4.2.1/cwctype /usr/include/wctype.h \
  /usr/include/c++/4.2.1/bits/ctype_base.h \
  /usr/include/c++/4.2.1/bits/ctype_inline.h \
  /usr/include/c++/4.2.1/bits/codecvt.h \
  /usr/include/c++/4.2.1/bits/time_members.h \
  /usr/include/c++/4.2.1/bits/messages_members.h \
  /usr/include/c++/4.2.1/bits/basic_ios.tcc \
  /usr/include/c++/4.2.1/bits/ostream.tcc /usr/include/c++/4.2.1/locale \
  /usr/include/c++/4.2.1/bits/locale_facets.tcc \
  /usr/include/c++/4.2.1/typeinfo /usr/include/c++/4.2.1/istream \
  /usr/include/c++/4.2.1/bits/istream.tcc TSTypes.h Logger.h \
  ByteArrayBuffer.h ByteArray.h
ByteArray.o: ByteArray.cc /usr/include/c++/4.2.1/string \
  /usr/include/c++/4.2.1/bits/c++config.h \
  /usr/include/c++/4.2.1/bits/os_defines.h /usr/include/unistd.h \
  /usr/include/_types.h /usr/include/sys/_types.h \
  /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/machine/_types.h \
  /usr/include/i386/_types.h /usr/include/sys/unistd.h \
  /usr/include/Availability.h /usr/include/AvailabilityInternal.h \
  /usr/include/sys/select.h /usr/include/sys/appleapiopts.h \
  /usr/include/sys/_structs.h /usr/include/sys/_select.h \
  /usr/include/c++/4.2.1/bits/cpu_defines.h \
  /usr/include/c++/4.2.1/bits/stringfwd.h \
  /usr/include/c++/4.2.1/bits/char_traits.h \
  /usr/include/c++/4.2.1/cstring /usr/include/c++/4.2.1/cstddef \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stddef.h \
  /usr/include/string.h /usr/include/strings.h \
  /usr/include/c++/4.2.1/bits/stl_algobase.h \
  /usr/include/c++/4.2.1/climits \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /usr/include/limits.h /usr/include/machine/limits.h \
  /usr/include/i386/limits.h /usr/include/i386/_limits.h \
  /usr/include/sys/syslimits.h /usr/include/c++/4.2.1/cstdlib \
  /usr/include/stdlib.h /usr/include/sys/wait.h /usr/include/sys/signal.h \
  /usr/include/machine/signal.h /usr/include/i386/signal.h \
  /usr/include/i386/_structs.h /usr/include/machine/_structs.h \
  /usr/include/mach/i386/_structs.h /usr/include/sys/resource.h \
  /usr/include/machine/endian.h /usr/include/i386/endian.h \
  /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/i386/_types.h /usr/include/c++/4.2.1/iosfwd \
  /usr/include/c++/4.2.1/bits/c++locale.h /usr/include/c++/4.2.1/clocale \
  /usr/include/locale.h /usr/include/_locale.h \
  /usr/include/c++/4.2.1/cstdio /usr/include/stdio.h \
  /usr/include/c++/4.2.1/cstdarg \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  /usr/include/c++/4.2.1/bits/c++io.h /usr/include/c++/4.2.1/bits/gthr.h \
  /usr/include/c++/4.2.1/bits/gthr-default.h /usr/include/pthread.h \
  /usr/include/pthread_impl.h /usr/include/sched.h /usr/include/time.h \
  /usr/include/_structs.h /usr/include/c++/4.2.1/cctype \
  /usr/include/ctype.h /usr/include/runetype.h \
  /usr/include/c++/4.2.1/bits/postypes.h /usr/include/c++/4.2.1/cwchar \
  /usr/include/c++/4.2.1/ctime /usr/include/wchar.h \
  /usr/include/_wctype.h /usr/include/stdint.h \
  /usr/include/c++/4.2.1/bits/functexcept.h \
  /usr/include/c++/4.2.1/exception_defines.h \
  /usr/include/c++/4.2.1/bits/stl_pair.h \
  /usr/include/c++/4.2.1/bits/cpp_type_traits.h \
  /usr/include/c++/4.2.1/ext/type_traits.h /usr/include/c++/4.2.1/utility \
  /usr/include/c++/4.2.1/bits/stl_relops.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.2.1/bits/concept_check.h \
  /usr/include/c++/4.2.1/bits/stl_iterator.h \
  /usr/include/c++/4.2.1/debug/debug.h /usr/include/c++/4.2.1/memory \
  /usr/include/c++/4.2.1/bits/allocator.h \
  /usr/include/c++/4.2.1/bits/c++allocator.h \
  /usr/include/c++/4.2.1/ext/new_allocator.h /usr/include/c++/4.2.1/new \
  /usr/include/c++/4.2.1/exception \
  /usr/include/c++/4.2.1/bits/stl_construct.h \
  /usr/include/c++/4.2.1/bits/stl_uninitialized.h \
  /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h \
  /usr/include/c++/4.2.1/limits \
  /usr/include/c++/4.2.1/bits/ostream_insert.h \
  /usr/include/c++/4.2.1/bits/stl_function.h \
  /usr/include/c++/4.2.1/bits/basic_string.h \
  /usr/include/c++/4.2.1/ext/atomicity.h \
  /usr/include/c++/4.2.1/bits/atomic_word.h \
  /usr/include/c++/4.2.1/algorithm /usr/include/c++/4.2.1/bits/stl_algo.h \
  /usr/include/c++/4.2.1/bits/stl_heap.h \
  /usr/include/c++/4.2.1/bits/stl_tempbuf.h \
  /usr/include/c++/4.2.1/bits/basic_string.tcc ByteArray.h \
  /usr/include/c++/4.2.1/iostream /usr/include/c++/4.2.1/ostream \
  /usr/include/c++/4.2.1/ios /usr/include/c++/4.2.1/bits/localefwd.h \
  /usr/include/c++/4.2.1/bits/ios_base.h \
  /usr/include/c++/4.2.1/bits/locale_classes.h \
  /usr/include/c++/4.2.1/streambuf \
  /usr/include/c++/4.2.1/bits/streambuf.tcc \
  /usr/include/c++/4.2.1/bits/basic_ios.h \
  /usr/include/c++/4.2.1/bits/streambuf_iterator.h \
  /usr/include/c++/4.2.1/bits/locale_facets.h \
  /usr/include/c++/4.2.1/cwctype /usr/include/wctype.h \
  /usr/include/c++/4.2.1/bits/ctype_base.h \
  /usr/include/c++/4.2.1/bits/ctype_inline.h \
  /usr/include/c++/4.2.1/bits/codecvt.h \
  /usr/include/c++/4.2.1/bits/time_members.h \
  /usr/include/c++/4.2.1/bits/messages_members.h \
  /usr/include/c++/4.2.1/bits/basic_ios.tcc \
  /usr/include/c++/4.2.1/bits/ostream.tcc /usr/include/c++/4.2.1/locale \
  /usr/include/c++/4.2.1/bits/locale_facets.tcc \
  /usr/include/c++/4.2.1/typeinfo /usr/include/c++/4.2.1/istream \
  /usr/include/c++/4.2.1/bits/istream.tcc /usr/include/assert.h TSTypes.h \
  Logger.h ByteArrayBuffer.h
ByteArrayBuffer.o: ByteArrayBuffer.cc /usr/include/string.h \
  /usr/include/_types.h /usr/include/sys/_types.h \
  /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/machine/_types.h \
  /usr/include/i386/_types.h /usr/include/Availability.h \
  /usr/include/AvailabilityInternal.h /usr/include/strings.h \
  ByteArrayBuffer.h ByteArray.h /usr/include/c++/4.2.1/iostream \
  /usr/include/c++/4.2.1/bits/c++config.h \
  /usr/include/c++/4.2.1/bits/os_defines.h /usr/include/unistd.h \
  /usr/include/sys/unistd.h /usr/include/sys/select.h \
  /usr/include/sys/appleapiopts.h /usr/include/sys/_structs.h \
  /usr/include/sys/_select.h /usr/include/c++/4.2.1/bits/cpu_defines.h \
  /usr/include/c++/4.2.1/ostream /usr/include/c++/4.2.1/ios \
  /usr/include/c++/4.2.1/iosfwd /usr/include/c++/4.2.1/bits/c++locale.h \
  /usr/include/c++/4.2.1/clocale /usr/include/locale.h \
  /usr/include/_locale.h /usr/include/c++/4.2.1/cstring \
  /usr/include/c++/4.2.1/cstddef \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stddef.h \
  /usr/include/c++/4.2.1/cstdio /usr/include/stdio.h \
  /usr/include/c++/4.2.1/cstdarg \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  /usr/include/c++/4.2.1/bits/c++io.h /usr/include/c++/4.2.1/bits/gthr.h \
  /usr/include/c++/4.2.1/bits/gthr-default.h /usr/include/pthread.h \
  /usr/include/pthread_impl.h /usr/include/sched.h /usr/include/time.h \
  /usr/include/_structs.h /usr/include/c++/4.2.1/cctype \
  /usr/include/ctype.h /usr/include/runetype.h \
  /usr/include/c++/4.2.1/bits/stringfwd.h \
  /usr/include/c++/4.2.1/bits/postypes.h /usr/include/c++/4.2.1/cwchar \
  /usr/include/c++/4.2.1/ctime /usr/include/wchar.h \
  /usr/include/_wctype.h /usr/include/stdint.h \
  /usr/include/c++/4.2.1/bits/functexcept.h \
  /usr/include/c++/4.2.1/exception_defines.h \
  /usr/include/c++/4.2.1/exception \
  /usr/include/c++/4.2.1/bits/char_traits.h \
  /usr/include/c++/4.2.1/bits/stl_algobase.h \
  /usr/include/c++/4.2.1/climits \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /usr/include/limits.h /usr/include/machine/limits.h \
  /usr/include/i386/limits.h /usr/include/i386/_limits.h \
  /usr/include/sys/syslimits.h /usr/include/c++/4.2.1/cstdlib \
  /usr/include/stdlib.h /usr/include/sys/wait.h /usr/include/sys/signal.h \
  /usr/include/machine/signal.h /usr/include/i386/signal.h \
  /usr/include/i386/_structs.h /usr/include/machine/_structs.h \
  /usr/include/mach/i386/_structs.h /usr/include/sys/resource.h \
  /usr/include/machine/endian.h /usr/include/i386/endian.h \
  /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/i386/_types.h /usr/include/c++/4.2.1/bits/stl_pair.h \
  /usr/include/c++/4.2.1/bits/cpp_type_traits.h \
  /usr/include/c++/4.2.1/ext/type_traits.h /usr/include/c++/4.2.1/utility \
  /usr/include/c++/4.2.1/bits/stl_relops.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.2.1/bits/concept_check.h \
  /usr/include/c++/4.2.1/bits/stl_iterator.h \
  /usr/include/c++/4.2.1/debug/debug.h \
  /usr/include/c++/4.2.1/bits/localefwd.h \
  /usr/include/c++/4.2.1/bits/ios_base.h \
  /usr/include/c++/4.2.1/ext/atomicity.h \
  /usr/include/c++/4.2.1/bits/atomic_word.h \
  /usr/include/c++/4.2.1/bits/locale_classes.h \
  /usr/include/c++/4.2.1/string /usr/include/c++/4.2.1/memory \
  /usr/include/c++/4.2.1/bits/allocator.h \
  /usr/include/c++/4.2.1/bits/c++allocator.h \
  /usr/include/c++/4.2.1/ext/new_allocator.h /usr/include/c++/4.2.1/new \
  /usr/include/c++/4.2.1/bits/stl_construct.h \
  /usr/include/c++/4.2.1/bits/stl_uninitialized.h \
  /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h \
  /usr/include/c++/4.2.1/limits \
  /usr/include/c++/4.2.1/bits/ostream_insert.h \
  /usr/include/c++/4.2.1/bits/stl_function.h \
  /usr/include/c++/4.2.1/bits/basic_string.h \
  /usr/include/c++/4.2.1/algorithm /usr/include/c++/4.2.1/bits/stl_algo.h \
  /usr/include/c++/4.2.1/bits/stl_heap.h \
  /usr/include/c++/4.2.1/bits/stl_tempbuf.h \
  /usr/include/c++/4.2.1/bits/basic_string.tcc \
  /usr/include/c++/4.2.1/streambuf \
  /usr/include/c++/4.2.1/bits/streambuf.tcc \
  /usr/include/c++/4.2.1/bits/basic_ios.h \
  /usr/include/c++/4.2.1/bits/streambuf_iterator.h \
  /usr/include/c++/4.2.1/bits/locale_facets.h \
  /usr/include/c++/4.2.1/cwctype /usr/include/wctype.h \
  /usr/include/c++/4.2.1/bits/ctype_base.h \
  /usr/include/c++/4.2.1/bits/ctype_inline.h \
  /usr/include/c++/4.2.1/bits/codecvt.h \
  /usr/include/c++/4.2.1/bits/time_members.h \
  /usr/include/c++/4.2.1/bits/messages_members.h \
  /usr/include/c++/4.2.1/bits/basic_ios.tcc \
  /usr/include/c++/4.2.1/bits/ostream.tcc /usr/include/c++/4.2.1/locale \
  /usr/include/c++/4.2.1/bits/locale_facets.tcc \
  /usr/include/c++/4.2.1/typeinfo /usr/include/c++/4.2.1/istream \
  /usr/include/c++/4.2.1/bits/istream.tcc /usr/include/assert.h TSTypes.h \
  Logger.h
Descriptors.o: Descriptors.cc /usr/include/assert.h \
  /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/stdlib.h \
  /usr/include/Availability.h /usr/include/AvailabilityInternal.h \
  /usr/include/_types.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/i386/_types.h \
  /usr/include/sys/wait.h /usr/include/sys/signal.h \
  /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h \
  /usr/include/i386/signal.h /usr/include/i386/_structs.h \
  /usr/include/sys/_structs.h /usr/include/machine/_structs.h \
  /usr/include/mach/i386/_structs.h /usr/include/sys/resource.h \
  /usr/include/machine/endian.h /usr/include/i386/endian.h \
  /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/i386/_types.h /usr/include/stdio.h \
  /usr/include/c++/4.2.1/iostream /usr/include/c++/4.2.1/bits/c++config.h \
  /usr/include/c++/4.2.1/bits/os_defines.h /usr/include/unistd.h \
  /usr/include/sys/unistd.h /usr/include/sys/select.h \
  /usr/include/sys/_select.h /usr/include/c++/4.2.1/bits/cpu_defines.h \
  /usr/include/c++/4.2.1/ostream /usr/include/c++/4.2.1/ios \
  /usr/include/c++/4.2.1/iosfwd /usr/include/c++/4.2.1/bits/c++locale.h \
  /usr/include/c++/4.2.1/clocale /usr/include/locale.h \
  /usr/include/_locale.h /usr/include/c++/4.2.1/cstring \
  /usr/include/c++/4.2.1/cstddef \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stddef.h \
  /usr/include/string.h /usr/include/strings.h \
  /usr/include/c++/4.2.1/cstdio /usr/include/c++/4.2.1/cstdarg \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  /usr/include/c++/4.2.1/bits/c++io.h /usr/include/c++/4.2.1/bits/gthr.h \
  /usr/include/c++/4.2.1/bits/gthr-default.h /usr/include/pthread.h \
  /usr/include/pthread_impl.h /usr/include/sched.h /usr/include/time.h \
  /usr/include/_structs.h /usr/include/c++/4.2.1/cctype \
  /usr/include/ctype.h /usr/include/runetype.h \
  /usr/include/c++/4.2.1/bits/stringfwd.h \
  /usr/include/c++/4.2.1/bits/postypes.h /usr/include/c++/4.2.1/cwchar \
  /usr/include/c++/4.2.1/ctime /usr/include/wchar.h \
  /usr/include/_wctype.h /usr/include/stdint.h \
  /usr/include/c++/4.2.1/bits/functexcept.h \
  /usr/include/c++/4.2.1/exception_defines.h \
  /usr/include/c++/4.2.1/exception \
  /usr/include/c++/4.2.1/bits/char_traits.h \
  /usr/include/c++/4.2.1/bits/stl_algobase.h \
  /usr/include/c++/4.2.1/climits \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /usr/include/limits.h /usr/include/machine/limits.h \
  /usr/include/i386/limits.h /usr/include/i386/_limits.h \
  /usr/include/sys/syslimits.h /usr/include/c++/4.2.1/cstdlib \
  /usr/include/c++/4.2.1/bits/stl_pair.h \
  /usr/include/c++/4.2.1/bits/cpp_type_traits.h \
  /usr/include/c++/4.2.1/ext/type_traits.h /usr/include/c++/4.2.1/utility \
  /usr/include/c++/4.2.1/bits/stl_relops.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.2.1/bits/concept_check.h \
  /usr/include/c++/4.2.1/bits/stl_iterator.h \
  /usr/include/c++/4.2.1/debug/debug.h \
  /usr/include/c++/4.2.1/bits/localefwd.h \
  /usr/include/c++/4.2.1/bits/ios_base.h \
  /usr/include/c++/4.2.1/ext/atomicity.h \
  /usr/include/c++/4.2.1/bits/atomic_word.h \
  /usr/include/c++/4.2.1/bits/locale_classes.h \
  /usr/include/c++/4.2.1/string /usr/include/c++/4.2.1/memory \
  /usr/include/c++/4.2.1/bits/allocator.h \
  /usr/include/c++/4.2.1/bits/c++allocator.h \
  /usr/include/c++/4.2.1/ext/new_allocator.h /usr/include/c++/4.2.1/new \
  /usr/include/c++/4.2.1/bits/stl_construct.h \
  /usr/include/c++/4.2.1/bits/stl_uninitialized.h \
  /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h \
  /usr/include/c++/4.2.1/limits \
  /usr/include/c++/4.2.1/bits/ostream_insert.h \
  /usr/include/c++/4.2.1/bits/stl_function.h \
  /usr/include/c++/4.2.1/bits/basic_string.h \
  /usr/include/c++/4.2.1/algorithm /usr/include/c++/4.2.1/bits/stl_algo.h \
  /usr/include/c++/4.2.1/bits/stl_heap.h \
  /usr/include/c++/4.2.1/bits/stl_tempbuf.h \
  /usr/include/c++/4.2.1/bits/basic_string.tcc \
  /usr/include/c++/4.2.1/streambuf \
  /usr/include/c++/4.2.1/bits/streambuf.tcc \
  /usr/include/c++/4.2.1/bits/basic_ios.h \
  /usr/include/c++/4.2.1/bits/streambuf_iterator.h \
  /usr/include/c++/4.2.1/bits/locale_facets.h \
  /usr/include/c++/4.2.1/cwctype /usr/include/wctype.h \
  /usr/include/c++/4.2.1/bits/ctype_base.h \
  /usr/include/c++/4.2.1/bits/ctype_inline.h \
  /usr/include/c++/4.2.1/bits/codecvt.h \
  /usr/include/c++/4.2.1/bits/time_members.h \
  /usr/include/c++/4.2.1/bits/messages_members.h \
  /usr/include/c++/4.2.1/bits/basic_ios.tcc \
  /usr/include/c++/4.2.1/bits/ostream.tcc /usr/include/c++/4.2.1/locale \
  /usr/include/c++/4.2.1/bits/locale_facets.tcc \
  /usr/include/c++/4.2.1/typeinfo /usr/include/c++/4.2.1/istream \
  /usr/include/c++/4.2.1/bits/istream.tcc /usr/include/c++/4.2.1/map \
  /usr/include/c++/4.2.1/bits/stl_tree.h \
  /usr/include/c++/4.2.1/bits/stl_map.h \
  /usr/include/c++/4.2.1/bits/stl_multimap.h Descriptors.h TSTypes.h \
  Logger.h BitStream.h ByteArrayBuffer.h ByteArray.h
EventInformationTable.o: EventInformationTable.cc /usr/include/assert.h \
  /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/stdlib.h \
  /usr/include/Availability.h /usr/include/AvailabilityInternal.h \
  /usr/include/_types.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/i386/_types.h \
  /usr/include/sys/wait.h /usr/include/sys/signal.h \
  /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h \
  /usr/include/i386/signal.h /usr/include/i386/_structs.h \
  /usr/include/sys/_structs.h /usr/include/machine/_structs.h \
  /usr/include/mach/i386/_structs.h /usr/include/sys/resource.h \
  /usr/include/machine/endian.h /usr/include/i386/endian.h \
  /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/i386/_types.h /usr/include/stdio.h \
  /usr/include/c++/4.2.1/iostream /usr/include/c++/4.2.1/bits/c++config.h \
  /usr/include/c++/4.2.1/bits/os_defines.h /usr/include/unistd.h \
  /usr/include/sys/unistd.h /usr/include/sys/select.h \
  /usr/include/sys/_select.h /usr/include/c++/4.2.1/bits/cpu_defines.h \
  /usr/include/c++/4.2.1/ostream /usr/include/c++/4.2.1/ios \
  /usr/include/c++/4.2.1/iosfwd /usr/include/c++/4.2.1/bits/c++locale.h \
  /usr/include/c++/4.2.1/clocale /usr/include/locale.h \
  /usr/include/_locale.h /usr/include/c++/4.2.1/cstring \
  /usr/include/c++/4.2.1/cstddef \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stddef.h \
  /usr/include/string.h /usr/include/strings.h \
  /usr/include/c++/4.2.1/cstdio /usr/include/c++/4.2.1/cstdarg \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  /usr/include/c++/4.2.1/bits/c++io.h /usr/include/c++/4.2.1/bits/gthr.h \
  /usr/include/c++/4.2.1/bits/gthr-default.h /usr/include/pthread.h \
  /usr/include/pthread_impl.h /usr/include/sched.h /usr/include/time.h \
  /usr/include/_structs.h /usr/include/c++/4.2.1/cctype \
  /usr/include/ctype.h /usr/include/runetype.h \
  /usr/include/c++/4.2.1/bits/stringfwd.h \
  /usr/include/c++/4.2.1/bits/postypes.h /usr/include/c++/4.2.1/cwchar \
  /usr/include/c++/4.2.1/ctime /usr/include/wchar.h \
  /usr/include/_wctype.h /usr/include/stdint.h \
  /usr/include/c++/4.2.1/bits/functexcept.h \
  /usr/include/c++/4.2.1/exception_defines.h \
  /usr/include/c++/4.2.1/exception \
  /usr/include/c++/4.2.1/bits/char_traits.h \
  /usr/include/c++/4.2.1/bits/stl_algobase.h \
  /usr/include/c++/4.2.1/climits \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /usr/include/limits.h /usr/include/machine/limits.h \
  /usr/include/i386/limits.h /usr/include/i386/_limits.h \
  /usr/include/sys/syslimits.h /usr/include/c++/4.2.1/cstdlib \
  /usr/include/c++/4.2.1/bits/stl_pair.h \
  /usr/include/c++/4.2.1/bits/cpp_type_traits.h \
  /usr/include/c++/4.2.1/ext/type_traits.h /usr/include/c++/4.2.1/utility \
  /usr/include/c++/4.2.1/bits/stl_relops.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.2.1/bits/concept_check.h \
  /usr/include/c++/4.2.1/bits/stl_iterator.h \
  /usr/include/c++/4.2.1/debug/debug.h \
  /usr/include/c++/4.2.1/bits/localefwd.h \
  /usr/include/c++/4.2.1/bits/ios_base.h \
  /usr/include/c++/4.2.1/ext/atomicity.h \
  /usr/include/c++/4.2.1/bits/atomic_word.h \
  /usr/include/c++/4.2.1/bits/locale_classes.h \
  /usr/include/c++/4.2.1/string /usr/include/c++/4.2.1/memory \
  /usr/include/c++/4.2.1/bits/allocator.h \
  /usr/include/c++/4.2.1/bits/c++allocator.h \
  /usr/include/c++/4.2.1/ext/new_allocator.h /usr/include/c++/4.2.1/new \
  /usr/include/c++/4.2.1/bits/stl_construct.h \
  /usr/include/c++/4.2.1/bits/stl_uninitialized.h \
  /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h \
  /usr/include/c++/4.2.1/limits \
  /usr/include/c++/4.2.1/bits/ostream_insert.h \
  /usr/include/c++/4.2.1/bits/stl_function.h \
  /usr/include/c++/4.2.1/bits/basic_string.h \
  /usr/include/c++/4.2.1/algorithm /usr/include/c++/4.2.1/bits/stl_algo.h \
  /usr/include/c++/4.2.1/bits/stl_heap.h \
  /usr/include/c++/4.2.1/bits/stl_tempbuf.h \
  /usr/include/c++/4.2.1/bits/basic_string.tcc \
  /usr/include/c++/4.2.1/streambuf \
  /usr/include/c++/4.2.1/bits/streambuf.tcc \
  /usr/include/c++/4.2.1/bits/basic_ios.h \
  /usr/include/c++/4.2.1/bits/streambuf_iterator.h \
  /usr/include/c++/4.2.1/bits/locale_facets.h \
  /usr/include/c++/4.2.1/cwctype /usr/include/wctype.h \
  /usr/include/c++/4.2.1/bits/ctype_base.h \
  /usr/include/c++/4.2.1/bits/ctype_inline.h \
  /usr/include/c++/4.2.1/bits/codecvt.h \
  /usr/include/c++/4.2.1/bits/time_members.h \
  /usr/include/c++/4.2.1/bits/messages_members.h \
  /usr/include/c++/4.2.1/bits/basic_ios.tcc \
  /usr/include/c++/4.2.1/bits/ostream.tcc /usr/include/c++/4.2.1/locale \
  /usr/include/c++/4.2.1/bits/locale_facets.tcc \
  /usr/include/c++/4.2.1/typeinfo /usr/include/c++/4.2.1/istream \
  /usr/include/c++/4.2.1/bits/istream.tcc Descriptors.h \
  /usr/include/c++/4.2.1/map /usr/include/c++/4.2.1/bits/stl_tree.h \
  /usr/include/c++/4.2.1/bits/stl_map.h \
  /usr/include/c++/4.2.1/bits/stl_multimap.h TSTypes.h Logger.h \
  BitStream.h ByteArrayBuffer.h ByteArray.h EventInformationTable.h \
  Table.h MJD.h
MJD.o: MJD.cc /usr/include/stdio.h /usr/include/sys/cdefs.h \
  /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/Availability.h \
  /usr/include/AvailabilityInternal.h /usr/include/_types.h \
  /usr/include/sys/_types.h /usr/include/machine/_types.h \
  /usr/include/i386/_types.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  MJD.h /usr/include/c++/4.2.1/ctime /usr/include/c++/4.2.1/cstddef \
  /usr/include/c++/4.2.1/bits/c++config.h \
  /usr/include/c++/4.2.1/bits/os_defines.h /usr/include/unistd.h \
  /usr/include/sys/unistd.h /usr/include/sys/select.h \
  /usr/include/sys/appleapiopts.h /usr/include/sys/_structs.h \
  /usr/include/sys/_select.h /usr/include/c++/4.2.1/bits/cpu_defines.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stddef.h \
  /usr/include/time.h /usr/include/_structs.h TSTypes.h Logger.h
PSI.o: PSI.cc /usr/include/assert.h /usr/include/sys/cdefs.h \
  /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/stdlib.h \
  /usr/include/Availability.h /usr/include/AvailabilityInternal.h \
  /usr/include/_types.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/i386/_types.h \
  /usr/include/sys/wait.h /usr/include/sys/signal.h \
  /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h \
  /usr/include/i386/signal.h /usr/include/i386/_structs.h \
  /usr/include/sys/_structs.h /usr/include/machine/_structs.h \
  /usr/include/mach/i386/_structs.h /usr/include/sys/resource.h \
  /usr/include/machine/endian.h /usr/include/i386/endian.h \
  /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/i386/_types.h /usr/include/c++/4.2.1/iostream \
  /usr/include/c++/4.2.1/bits/c++config.h \
  /usr/include/c++/4.2.1/bits/os_defines.h /usr/include/unistd.h \
  /usr/include/sys/unistd.h /usr/include/sys/select.h \
  /usr/include/sys/_select.h /usr/include/c++/4.2.1/bits/cpu_defines.h \
  /usr/include/c++/4.2.1/ostream /usr/include/c++/4.2.1/ios \
  /usr/include/c++/4.2.1/iosfwd /usr/include/c++/4.2.1/bits/c++locale.h \
  /usr/include/c++/4.2.1/clocale /usr/include/locale.h \
  /usr/include/_locale.h /usr/include/c++/4.2.1/cstring \
  /usr/include/c++/4.2.1/cstddef \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stddef.h \
  /usr/include/string.h /usr/include/strings.h \
  /usr/include/c++/4.2.1/cstdio /usr/include/stdio.h \
  /usr/include/c++/4.2.1/cstdarg \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  /usr/include/c++/4.2.1/bits/c++io.h /usr/include/c++/4.2.1/bits/gthr.h \
  /usr/include/c++/4.2.1/bits/gthr-default.h /usr/include/pthread.h \
  /usr/include/pthread_impl.h /usr/include/sched.h /usr/include/time.h \
  /usr/include/_structs.h /usr/include/c++/4.2.1/cctype \
  /usr/include/ctype.h /usr/include/runetype.h \
  /usr/include/c++/4.2.1/bits/stringfwd.h \
  /usr/include/c++/4.2.1/bits/postypes.h /usr/include/c++/4.2.1/cwchar \
  /usr/include/c++/4.2.1/ctime /usr/include/wchar.h \
  /usr/include/_wctype.h /usr/include/stdint.h \
  /usr/include/c++/4.2.1/bits/functexcept.h \
  /usr/include/c++/4.2.1/exception_defines.h \
  /usr/include/c++/4.2.1/exception \
  /usr/include/c++/4.2.1/bits/char_traits.h \
  /usr/include/c++/4.2.1/bits/stl_algobase.h \
  /usr/include/c++/4.2.1/climits \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /usr/include/limits.h /usr/include/machine/limits.h \
  /usr/include/i386/limits.h /usr/include/i386/_limits.h \
  /usr/include/sys/syslimits.h /usr/include/c++/4.2.1/cstdlib \
  /usr/include/c++/4.2.1/bits/stl_pair.h \
  /usr/include/c++/4.2.1/bits/cpp_type_traits.h \
  /usr/include/c++/4.2.1/ext/type_traits.h /usr/include/c++/4.2.1/utility \
  /usr/include/c++/4.2.1/bits/stl_relops.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.2.1/bits/concept_check.h \
  /usr/include/c++/4.2.1/bits/stl_iterator.h \
  /usr/include/c++/4.2.1/debug/debug.h \
  /usr/include/c++/4.2.1/bits/localefwd.h \
  /usr/include/c++/4.2.1/bits/ios_base.h \
  /usr/include/c++/4.2.1/ext/atomicity.h \
  /usr/include/c++/4.2.1/bits/atomic_word.h \
  /usr/include/c++/4.2.1/bits/locale_classes.h \
  /usr/include/c++/4.2.1/string /usr/include/c++/4.2.1/memory \
  /usr/include/c++/4.2.1/bits/allocator.h \
  /usr/include/c++/4.2.1/bits/c++allocator.h \
  /usr/include/c++/4.2.1/ext/new_allocator.h /usr/include/c++/4.2.1/new \
  /usr/include/c++/4.2.1/bits/stl_construct.h \
  /usr/include/c++/4.2.1/bits/stl_uninitialized.h \
  /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h \
  /usr/include/c++/4.2.1/limits \
  /usr/include/c++/4.2.1/bits/ostream_insert.h \
  /usr/include/c++/4.2.1/bits/stl_function.h \
  /usr/include/c++/4.2.1/bits/basic_string.h \
  /usr/include/c++/4.2.1/algorithm /usr/include/c++/4.2.1/bits/stl_algo.h \
  /usr/include/c++/4.2.1/bits/stl_heap.h \
  /usr/include/c++/4.2.1/bits/stl_tempbuf.h \
  /usr/include/c++/4.2.1/bits/basic_string.tcc \
  /usr/include/c++/4.2.1/streambuf \
  /usr/include/c++/4.2.1/bits/streambuf.tcc \
  /usr/include/c++/4.2.1/bits/basic_ios.h \
  /usr/include/c++/4.2.1/bits/streambuf_iterator.h \
  /usr/include/c++/4.2.1/bits/locale_facets.h \
  /usr/include/c++/4.2.1/cwctype /usr/include/wctype.h \
  /usr/include/c++/4.2.1/bits/ctype_base.h \
  /usr/include/c++/4.2.1/bits/ctype_inline.h \
  /usr/include/c++/4.2.1/bits/codecvt.h \
  /usr/include/c++/4.2.1/bits/time_members.h \
  /usr/include/c++/4.2.1/bits/messages_members.h \
  /usr/include/c++/4.2.1/bits/basic_ios.tcc \
  /usr/include/c++/4.2.1/bits/ostream.tcc /usr/include/c++/4.2.1/locale \
  /usr/include/c++/4.2.1/bits/locale_facets.tcc \
  /usr/include/c++/4.2.1/typeinfo /usr/include/c++/4.2.1/istream \
  /usr/include/c++/4.2.1/bits/istream.tcc PSI.h BitStream.h TSTypes.h \
  Logger.h ByteArrayBuffer.h ByteArray.h
ProgramAssociationSection.o: ProgramAssociationSection.cc \
  /usr/include/assert.h /usr/include/sys/cdefs.h \
  /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/stdlib.h \
  /usr/include/Availability.h /usr/include/AvailabilityInternal.h \
  /usr/include/_types.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/i386/_types.h \
  /usr/include/sys/wait.h /usr/include/sys/signal.h \
  /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h \
  /usr/include/i386/signal.h /usr/include/i386/_structs.h \
  /usr/include/sys/_structs.h /usr/include/machine/_structs.h \
  /usr/include/mach/i386/_structs.h /usr/include/sys/resource.h \
  /usr/include/machine/endian.h /usr/include/i386/endian.h \
  /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/i386/_types.h /usr/include/c++/4.2.1/iostream \
  /usr/include/c++/4.2.1/bits/c++config.h \
  /usr/include/c++/4.2.1/bits/os_defines.h /usr/include/unistd.h \
  /usr/include/sys/unistd.h /usr/include/sys/select.h \
  /usr/include/sys/_select.h /usr/include/c++/4.2.1/bits/cpu_defines.h \
  /usr/include/c++/4.2.1/ostream /usr/include/c++/4.2.1/ios \
  /usr/include/c++/4.2.1/iosfwd /usr/include/c++/4.2.1/bits/c++locale.h \
  /usr/include/c++/4.2.1/clocale /usr/include/locale.h \
  /usr/include/_locale.h /usr/include/c++/4.2.1/cstring \
  /usr/include/c++/4.2.1/cstddef \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stddef.h \
  /usr/include/string.h /usr/include/strings.h \
  /usr/include/c++/4.2.1/cstdio /usr/include/stdio.h \
  /usr/include/c++/4.2.1/cstdarg \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  /usr/include/c++/4.2.1/bits/c++io.h /usr/include/c++/4.2.1/bits/gthr.h \
  /usr/include/c++/4.2.1/bits/gthr-default.h /usr/include/pthread.h \
  /usr/include/pthread_impl.h /usr/include/sched.h /usr/include/time.h \
  /usr/include/_structs.h /usr/include/c++/4.2.1/cctype \
  /usr/include/ctype.h /usr/include/runetype.h \
  /usr/include/c++/4.2.1/bits/stringfwd.h \
  /usr/include/c++/4.2.1/bits/postypes.h /usr/include/c++/4.2.1/cwchar \
  /usr/include/c++/4.2.1/ctime /usr/include/wchar.h \
  /usr/include/_wctype.h /usr/include/stdint.h \
  /usr/include/c++/4.2.1/bits/functexcept.h \
  /usr/include/c++/4.2.1/exception_defines.h \
  /usr/include/c++/4.2.1/exception \
  /usr/include/c++/4.2.1/bits/char_traits.h \
  /usr/include/c++/4.2.1/bits/stl_algobase.h \
  /usr/include/c++/4.2.1/climits \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /usr/include/limits.h /usr/include/machine/limits.h \
  /usr/include/i386/limits.h /usr/include/i386/_limits.h \
  /usr/include/sys/syslimits.h /usr/include/c++/4.2.1/cstdlib \
  /usr/include/c++/4.2.1/bits/stl_pair.h \
  /usr/include/c++/4.2.1/bits/cpp_type_traits.h \
  /usr/include/c++/4.2.1/ext/type_traits.h /usr/include/c++/4.2.1/utility \
  /usr/include/c++/4.2.1/bits/stl_relops.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.2.1/bits/concept_check.h \
  /usr/include/c++/4.2.1/bits/stl_iterator.h \
  /usr/include/c++/4.2.1/debug/debug.h \
  /usr/include/c++/4.2.1/bits/localefwd.h \
  /usr/include/c++/4.2.1/bits/ios_base.h \
  /usr/include/c++/4.2.1/ext/atomicity.h \
  /usr/include/c++/4.2.1/bits/atomic_word.h \
  /usr/include/c++/4.2.1/bits/locale_classes.h \
  /usr/include/c++/4.2.1/string /usr/include/c++/4.2.1/memory \
  /usr/include/c++/4.2.1/bits/allocator.h \
  /usr/include/c++/4.2.1/bits/c++allocator.h \
  /usr/include/c++/4.2.1/ext/new_allocator.h /usr/include/c++/4.2.1/new \
  /usr/include/c++/4.2.1/bits/stl_construct.h \
  /usr/include/c++/4.2.1/bits/stl_uninitialized.h \
  /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h \
  /usr/include/c++/4.2.1/limits \
  /usr/include/c++/4.2.1/bits/ostream_insert.h \
  /usr/include/c++/4.2.1/bits/stl_function.h \
  /usr/include/c++/4.2.1/bits/basic_string.h \
  /usr/include/c++/4.2.1/algorithm /usr/include/c++/4.2.1/bits/stl_algo.h \
  /usr/include/c++/4.2.1/bits/stl_heap.h \
  /usr/include/c++/4.2.1/bits/stl_tempbuf.h \
  /usr/include/c++/4.2.1/bits/basic_string.tcc \
  /usr/include/c++/4.2.1/streambuf \
  /usr/include/c++/4.2.1/bits/streambuf.tcc \
  /usr/include/c++/4.2.1/bits/basic_ios.h \
  /usr/include/c++/4.2.1/bits/streambuf_iterator.h \
  /usr/include/c++/4.2.1/bits/locale_facets.h \
  /usr/include/c++/4.2.1/cwctype /usr/include/wctype.h \
  /usr/include/c++/4.2.1/bits/ctype_base.h \
  /usr/include/c++/4.2.1/bits/ctype_inline.h \
  /usr/include/c++/4.2.1/bits/codecvt.h \
  /usr/include/c++/4.2.1/bits/time_members.h \
  /usr/include/c++/4.2.1/bits/messages_members.h \
  /usr/include/c++/4.2.1/bits/basic_ios.tcc \
  /usr/include/c++/4.2.1/bits/ostream.tcc /usr/include/c++/4.2.1/locale \
  /usr/include/c++/4.2.1/bits/locale_facets.tcc \
  /usr/include/c++/4.2.1/typeinfo /usr/include/c++/4.2.1/istream \
  /usr/include/c++/4.2.1/bits/istream.tcc ProgramAssociationSection.h \
  Table.h TSTypes.h Logger.h BitStream.h ByteArrayBuffer.h ByteArray.h
ProgramMapSection.o: ProgramMapSection.cc /usr/include/assert.h \
  /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/stdlib.h \
  /usr/include/Availability.h /usr/include/AvailabilityInternal.h \
  /usr/include/_types.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/i386/_types.h \
  /usr/include/sys/wait.h /usr/include/sys/signal.h \
  /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h \
  /usr/include/i386/signal.h /usr/include/i386/_structs.h \
  /usr/include/sys/_structs.h /usr/include/machine/_structs.h \
  /usr/include/mach/i386/_structs.h /usr/include/sys/resource.h \
  /usr/include/machine/endian.h /usr/include/i386/endian.h \
  /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/i386/_types.h /usr/include/stdio.h \
  /usr/include/c++/4.2.1/iostream /usr/include/c++/4.2.1/bits/c++config.h \
  /usr/include/c++/4.2.1/bits/os_defines.h /usr/include/unistd.h \
  /usr/include/sys/unistd.h /usr/include/sys/select.h \
  /usr/include/sys/_select.h /usr/include/c++/4.2.1/bits/cpu_defines.h \
  /usr/include/c++/4.2.1/ostream /usr/include/c++/4.2.1/ios \
  /usr/include/c++/4.2.1/iosfwd /usr/include/c++/4.2.1/bits/c++locale.h \
  /usr/include/c++/4.2.1/clocale /usr/include/locale.h \
  /usr/include/_locale.h /usr/include/c++/4.2.1/cstring \
  /usr/include/c++/4.2.1/cstddef \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stddef.h \
  /usr/include/string.h /usr/include/strings.h \
  /usr/include/c++/4.2.1/cstdio /usr/include/c++/4.2.1/cstdarg \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  /usr/include/c++/4.2.1/bits/c++io.h /usr/include/c++/4.2.1/bits/gthr.h \
  /usr/include/c++/4.2.1/bits/gthr-default.h /usr/include/pthread.h \
  /usr/include/pthread_impl.h /usr/include/sched.h /usr/include/time.h \
  /usr/include/_structs.h /usr/include/c++/4.2.1/cctype \
  /usr/include/ctype.h /usr/include/runetype.h \
  /usr/include/c++/4.2.1/bits/stringfwd.h \
  /usr/include/c++/4.2.1/bits/postypes.h /usr/include/c++/4.2.1/cwchar \
  /usr/include/c++/4.2.1/ctime /usr/include/wchar.h \
  /usr/include/_wctype.h /usr/include/stdint.h \
  /usr/include/c++/4.2.1/bits/functexcept.h \
  /usr/include/c++/4.2.1/exception_defines.h \
  /usr/include/c++/4.2.1/exception \
  /usr/include/c++/4.2.1/bits/char_traits.h \
  /usr/include/c++/4.2.1/bits/stl_algobase.h \
  /usr/include/c++/4.2.1/climits \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /usr/include/limits.h /usr/include/machine/limits.h \
  /usr/include/i386/limits.h /usr/include/i386/_limits.h \
  /usr/include/sys/syslimits.h /usr/include/c++/4.2.1/cstdlib \
  /usr/include/c++/4.2.1/bits/stl_pair.h \
  /usr/include/c++/4.2.1/bits/cpp_type_traits.h \
  /usr/include/c++/4.2.1/ext/type_traits.h /usr/include/c++/4.2.1/utility \
  /usr/include/c++/4.2.1/bits/stl_relops.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.2.1/bits/concept_check.h \
  /usr/include/c++/4.2.1/bits/stl_iterator.h \
  /usr/include/c++/4.2.1/debug/debug.h \
  /usr/include/c++/4.2.1/bits/localefwd.h \
  /usr/include/c++/4.2.1/bits/ios_base.h \
  /usr/include/c++/4.2.1/ext/atomicity.h \
  /usr/include/c++/4.2.1/bits/atomic_word.h \
  /usr/include/c++/4.2.1/bits/locale_classes.h \
  /usr/include/c++/4.2.1/string /usr/include/c++/4.2.1/memory \
  /usr/include/c++/4.2.1/bits/allocator.h \
  /usr/include/c++/4.2.1/bits/c++allocator.h \
  /usr/include/c++/4.2.1/ext/new_allocator.h /usr/include/c++/4.2.1/new \
  /usr/include/c++/4.2.1/bits/stl_construct.h \
  /usr/include/c++/4.2.1/bits/stl_uninitialized.h \
  /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h \
  /usr/include/c++/4.2.1/limits \
  /usr/include/c++/4.2.1/bits/ostream_insert.h \
  /usr/include/c++/4.2.1/bits/stl_function.h \
  /usr/include/c++/4.2.1/bits/basic_string.h \
  /usr/include/c++/4.2.1/algorithm /usr/include/c++/4.2.1/bits/stl_algo.h \
  /usr/include/c++/4.2.1/bits/stl_heap.h \
  /usr/include/c++/4.2.1/bits/stl_tempbuf.h \
  /usr/include/c++/4.2.1/bits/basic_string.tcc \
  /usr/include/c++/4.2.1/streambuf \
  /usr/include/c++/4.2.1/bits/streambuf.tcc \
  /usr/include/c++/4.2.1/bits/basic_ios.h \
  /usr/include/c++/4.2.1/bits/streambuf_iterator.h \
  /usr/include/c++/4.2.1/bits/locale_facets.h \
  /usr/include/c++/4.2.1/cwctype /usr/include/wctype.h \
  /usr/include/c++/4.2.1/bits/ctype_base.h \
  /usr/include/c++/4.2.1/bits/ctype_inline.h \
  /usr/include/c++/4.2.1/bits/codecvt.h \
  /usr/include/c++/4.2.1/bits/time_members.h \
  /usr/include/c++/4.2.1/bits/messages_members.h \
  /usr/include/c++/4.2.1/bits/basic_ios.tcc \
  /usr/include/c++/4.2.1/bits/ostream.tcc /usr/include/c++/4.2.1/locale \
  /usr/include/c++/4.2.1/bits/locale_facets.tcc \
  /usr/include/c++/4.2.1/typeinfo /usr/include/c++/4.2.1/istream \
  /usr/include/c++/4.2.1/bits/istream.tcc /usr/include/c++/4.2.1/map \
  /usr/include/c++/4.2.1/bits/stl_tree.h \
  /usr/include/c++/4.2.1/bits/stl_map.h \
  /usr/include/c++/4.2.1/bits/stl_multimap.h ProgramMapSection.h Table.h \
  TSTypes.h Logger.h BitStream.h ByteArrayBuffer.h ByteArray.h \
  ProgramAssociationSection.h Descriptors.h
ServiceDescriptionTable.o: ServiceDescriptionTable.cc \
  /usr/include/assert.h /usr/include/sys/cdefs.h \
  /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/stdlib.h \
  /usr/include/Availability.h /usr/include/AvailabilityInternal.h \
  /usr/include/_types.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/i386/_types.h \
  /usr/include/sys/wait.h /usr/include/sys/signal.h \
  /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h \
  /usr/include/i386/signal.h /usr/include/i386/_structs.h \
  /usr/include/sys/_structs.h /usr/include/machine/_structs.h \
  /usr/include/mach/i386/_structs.h /usr/include/sys/resource.h \
  /usr/include/machine/endian.h /usr/include/i386/endian.h \
  /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/i386/_types.h /usr/include/stdio.h \
  /usr/include/c++/4.2.1/iostream /usr/include/c++/4.2.1/bits/c++config.h \
  /usr/include/c++/4.2.1/bits/os_defines.h /usr/include/unistd.h \
  /usr/include/sys/unistd.h /usr/include/sys/select.h \
  /usr/include/sys/_select.h /usr/include/c++/4.2.1/bits/cpu_defines.h \
  /usr/include/c++/4.2.1/ostream /usr/include/c++/4.2.1/ios \
  /usr/include/c++/4.2.1/iosfwd /usr/include/c++/4.2.1/bits/c++locale.h \
  /usr/include/c++/4.2.1/clocale /usr/include/locale.h \
  /usr/include/_locale.h /usr/include/c++/4.2.1/cstring \
  /usr/include/c++/4.2.1/cstddef \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stddef.h \
  /usr/include/string.h /usr/include/strings.h \
  /usr/include/c++/4.2.1/cstdio /usr/include/c++/4.2.1/cstdarg \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  /usr/include/c++/4.2.1/bits/c++io.h /usr/include/c++/4.2.1/bits/gthr.h \
  /usr/include/c++/4.2.1/bits/gthr-default.h /usr/include/pthread.h \
  /usr/include/pthread_impl.h /usr/include/sched.h /usr/include/time.h \
  /usr/include/_structs.h /usr/include/c++/4.2.1/cctype \
  /usr/include/ctype.h /usr/include/runetype.h \
  /usr/include/c++/4.2.1/bits/stringfwd.h \
  /usr/include/c++/4.2.1/bits/postypes.h /usr/include/c++/4.2.1/cwchar \
  /usr/include/c++/4.2.1/ctime /usr/include/wchar.h \
  /usr/include/_wctype.h /usr/include/stdint.h \
  /usr/include/c++/4.2.1/bits/functexcept.h \
  /usr/include/c++/4.2.1/exception_defines.h \
  /usr/include/c++/4.2.1/exception \
  /usr/include/c++/4.2.1/bits/char_traits.h \
  /usr/include/c++/4.2.1/bits/stl_algobase.h \
  /usr/include/c++/4.2.1/climits \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /usr/include/limits.h /usr/include/machine/limits.h \
  /usr/include/i386/limits.h /usr/include/i386/_limits.h \
  /usr/include/sys/syslimits.h /usr/include/c++/4.2.1/cstdlib \
  /usr/include/c++/4.2.1/bits/stl_pair.h \
  /usr/include/c++/4.2.1/bits/cpp_type_traits.h \
  /usr/include/c++/4.2.1/ext/type_traits.h /usr/include/c++/4.2.1/utility \
  /usr/include/c++/4.2.1/bits/stl_relops.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.2.1/bits/concept_check.h \
  /usr/include/c++/4.2.1/bits/stl_iterator.h \
  /usr/include/c++/4.2.1/debug/debug.h \
  /usr/include/c++/4.2.1/bits/localefwd.h \
  /usr/include/c++/4.2.1/bits/ios_base.h \
  /usr/include/c++/4.2.1/ext/atomicity.h \
  /usr/include/c++/4.2.1/bits/atomic_word.h \
  /usr/include/c++/4.2.1/bits/locale_classes.h \
  /usr/include/c++/4.2.1/string /usr/include/c++/4.2.1/memory \
  /usr/include/c++/4.2.1/bits/allocator.h \
  /usr/include/c++/4.2.1/bits/c++allocator.h \
  /usr/include/c++/4.2.1/ext/new_allocator.h /usr/include/c++/4.2.1/new \
  /usr/include/c++/4.2.1/bits/stl_construct.h \
  /usr/include/c++/4.2.1/bits/stl_uninitialized.h \
  /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h \
  /usr/include/c++/4.2.1/limits \
  /usr/include/c++/4.2.1/bits/ostream_insert.h \
  /usr/include/c++/4.2.1/bits/stl_function.h \
  /usr/include/c++/4.2.1/bits/basic_string.h \
  /usr/include/c++/4.2.1/algorithm /usr/include/c++/4.2.1/bits/stl_algo.h \
  /usr/include/c++/4.2.1/bits/stl_heap.h \
  /usr/include/c++/4.2.1/bits/stl_tempbuf.h \
  /usr/include/c++/4.2.1/bits/basic_string.tcc \
  /usr/include/c++/4.2.1/streambuf \
  /usr/include/c++/4.2.1/bits/streambuf.tcc \
  /usr/include/c++/4.2.1/bits/basic_ios.h \
  /usr/include/c++/4.2.1/bits/streambuf_iterator.h \
  /usr/include/c++/4.2.1/bits/locale_facets.h \
  /usr/include/c++/4.2.1/cwctype /usr/include/wctype.h \
  /usr/include/c++/4.2.1/bits/ctype_base.h \
  /usr/include/c++/4.2.1/bits/ctype_inline.h \
  /usr/include/c++/4.2.1/bits/codecvt.h \
  /usr/include/c++/4.2.1/bits/time_members.h \
  /usr/include/c++/4.2.1/bits/messages_members.h \
  /usr/include/c++/4.2.1/bits/basic_ios.tcc \
  /usr/include/c++/4.2.1/bits/ostream.tcc /usr/include/c++/4.2.1/locale \
  /usr/include/c++/4.2.1/bits/locale_facets.tcc \
  /usr/include/c++/4.2.1/typeinfo /usr/include/c++/4.2.1/istream \
  /usr/include/c++/4.2.1/bits/istream.tcc Descriptors.h \
  /usr/include/c++/4.2.1/map /usr/include/c++/4.2.1/bits/stl_tree.h \
  /usr/include/c++/4.2.1/bits/stl_map.h \
  /usr/include/c++/4.2.1/bits/stl_multimap.h TSTypes.h Logger.h \
  BitStream.h ByteArrayBuffer.h ByteArray.h ServiceDescriptionTable.h \
  Table.h
StdLogger.o: StdLogger.cc /usr/include/stdio.h /usr/include/sys/cdefs.h \
  /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/Availability.h \
  /usr/include/AvailabilityInternal.h /usr/include/_types.h \
  /usr/include/sys/_types.h /usr/include/machine/_types.h \
  /usr/include/i386/_types.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  /usr/include/stdlib.h /usr/include/sys/wait.h /usr/include/sys/signal.h \
  /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h \
  /usr/include/i386/signal.h /usr/include/i386/_structs.h \
  /usr/include/sys/_structs.h /usr/include/machine/_structs.h \
  /usr/include/mach/i386/_structs.h /usr/include/sys/resource.h \
  /usr/include/machine/endian.h /usr/include/i386/endian.h \
  /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/i386/_types.h StdLogger.h Logger.h
Table.o: Table.cc /usr/include/assert.h /usr/include/sys/cdefs.h \
  /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/stdlib.h \
  /usr/include/Availability.h /usr/include/AvailabilityInternal.h \
  /usr/include/_types.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/i386/_types.h \
  /usr/include/sys/wait.h /usr/include/sys/signal.h \
  /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h \
  /usr/include/i386/signal.h /usr/include/i386/_structs.h \
  /usr/include/sys/_structs.h /usr/include/machine/_structs.h \
  /usr/include/mach/i386/_structs.h /usr/include/sys/resource.h \
  /usr/include/machine/endian.h /usr/include/i386/endian.h \
  /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/i386/_types.h /usr/include/c++/4.2.1/iostream \
  /usr/include/c++/4.2.1/bits/c++config.h \
  /usr/include/c++/4.2.1/bits/os_defines.h /usr/include/unistd.h \
  /usr/include/sys/unistd.h /usr/include/sys/select.h \
  /usr/include/sys/_select.h /usr/include/c++/4.2.1/bits/cpu_defines.h \
  /usr/include/c++/4.2.1/ostream /usr/include/c++/4.2.1/ios \
  /usr/include/c++/4.2.1/iosfwd /usr/include/c++/4.2.1/bits/c++locale.h \
  /usr/include/c++/4.2.1/clocale /usr/include/locale.h \
  /usr/include/_locale.h /usr/include/c++/4.2.1/cstring \
  /usr/include/c++/4.2.1/cstddef \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stddef.h \
  /usr/include/string.h /usr/include/strings.h \
  /usr/include/c++/4.2.1/cstdio /usr/include/stdio.h \
  /usr/include/c++/4.2.1/cstdarg \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  /usr/include/c++/4.2.1/bits/c++io.h /usr/include/c++/4.2.1/bits/gthr.h \
  /usr/include/c++/4.2.1/bits/gthr-default.h /usr/include/pthread.h \
  /usr/include/pthread_impl.h /usr/include/sched.h /usr/include/time.h \
  /usr/include/_structs.h /usr/include/c++/4.2.1/cctype \
  /usr/include/ctype.h /usr/include/runetype.h \
  /usr/include/c++/4.2.1/bits/stringfwd.h \
  /usr/include/c++/4.2.1/bits/postypes.h /usr/include/c++/4.2.1/cwchar \
  /usr/include/c++/4.2.1/ctime /usr/include/wchar.h \
  /usr/include/_wctype.h /usr/include/stdint.h \
  /usr/include/c++/4.2.1/bits/functexcept.h \
  /usr/include/c++/4.2.1/exception_defines.h \
  /usr/include/c++/4.2.1/exception \
  /usr/include/c++/4.2.1/bits/char_traits.h \
  /usr/include/c++/4.2.1/bits/stl_algobase.h \
  /usr/include/c++/4.2.1/climits \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /usr/include/limits.h /usr/include/machine/limits.h \
  /usr/include/i386/limits.h /usr/include/i386/_limits.h \
  /usr/include/sys/syslimits.h /usr/include/c++/4.2.1/cstdlib \
  /usr/include/c++/4.2.1/bits/stl_pair.h \
  /usr/include/c++/4.2.1/bits/cpp_type_traits.h \
  /usr/include/c++/4.2.1/ext/type_traits.h /usr/include/c++/4.2.1/utility \
  /usr/include/c++/4.2.1/bits/stl_relops.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.2.1/bits/concept_check.h \
  /usr/include/c++/4.2.1/bits/stl_iterator.h \
  /usr/include/c++/4.2.1/debug/debug.h \
  /usr/include/c++/4.2.1/bits/localefwd.h \
  /usr/include/c++/4.2.1/bits/ios_base.h \
  /usr/include/c++/4.2.1/ext/atomicity.h \
  /usr/include/c++/4.2.1/bits/atomic_word.h \
  /usr/include/c++/4.2.1/bits/locale_classes.h \
  /usr/include/c++/4.2.1/string /usr/include/c++/4.2.1/memory \
  /usr/include/c++/4.2.1/bits/allocator.h \
  /usr/include/c++/4.2.1/bits/c++allocator.h \
  /usr/include/c++/4.2.1/ext/new_allocator.h /usr/include/c++/4.2.1/new \
  /usr/include/c++/4.2.1/bits/stl_construct.h \
  /usr/include/c++/4.2.1/bits/stl_uninitialized.h \
  /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h \
  /usr/include/c++/4.2.1/limits \
  /usr/include/c++/4.2.1/bits/ostream_insert.h \
  /usr/include/c++/4.2.1/bits/stl_function.h \
  /usr/include/c++/4.2.1/bits/basic_string.h \
  /usr/include/c++/4.2.1/algorithm /usr/include/c++/4.2.1/bits/stl_algo.h \
  /usr/include/c++/4.2.1/bits/stl_heap.h \
  /usr/include/c++/4.2.1/bits/stl_tempbuf.h \
  /usr/include/c++/4.2.1/bits/basic_string.tcc \
  /usr/include/c++/4.2.1/streambuf \
  /usr/include/c++/4.2.1/bits/streambuf.tcc \
  /usr/include/c++/4.2.1/bits/basic_ios.h \
  /usr/include/c++/4.2.1/bits/streambuf_iterator.h \
  /usr/include/c++/4.2.1/bits/locale_facets.h \
  /usr/include/c++/4.2.1/cwctype /usr/include/wctype.h \
  /usr/include/c++/4.2.1/bits/ctype_base.h \
  /usr/include/c++/4.2.1/bits/ctype_inline.h \
  /usr/include/c++/4.2.1/bits/codecvt.h \
  /usr/include/c++/4.2.1/bits/time_members.h \
  /usr/include/c++/4.2.1/bits/messages_members.h \
  /usr/include/c++/4.2.1/bits/basic_ios.tcc \
  /usr/include/c++/4.2.1/bits/ostream.tcc /usr/include/c++/4.2.1/locale \
  /usr/include/c++/4.2.1/bits/locale_facets.tcc \
  /usr/include/c++/4.2.1/typeinfo /usr/include/c++/4.2.1/istream \
  /usr/include/c++/4.2.1/bits/istream.tcc Table.h TSTypes.h Logger.h \
  BitStream.h ByteArrayBuffer.h ByteArray.h
TimeDateSection.o: TimeDateSection.cc /usr/include/assert.h \
  /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/stdlib.h \
  /usr/include/Availability.h /usr/include/AvailabilityInternal.h \
  /usr/include/_types.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/i386/_types.h \
  /usr/include/sys/wait.h /usr/include/sys/signal.h \
  /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h \
  /usr/include/i386/signal.h /usr/include/i386/_structs.h \
  /usr/include/sys/_structs.h /usr/include/machine/_structs.h \
  /usr/include/mach/i386/_structs.h /usr/include/sys/resource.h \
  /usr/include/machine/endian.h /usr/include/i386/endian.h \
  /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/i386/_types.h /usr/include/stdio.h \
  /usr/include/c++/4.2.1/iostream /usr/include/c++/4.2.1/bits/c++config.h \
  /usr/include/c++/4.2.1/bits/os_defines.h /usr/include/unistd.h \
  /usr/include/sys/unistd.h /usr/include/sys/select.h \
  /usr/include/sys/_select.h /usr/include/c++/4.2.1/bits/cpu_defines.h \
  /usr/include/c++/4.2.1/ostream /usr/include/c++/4.2.1/ios \
  /usr/include/c++/4.2.1/iosfwd /usr/include/c++/4.2.1/bits/c++locale.h \
  /usr/include/c++/4.2.1/clocale /usr/include/locale.h \
  /usr/include/_locale.h /usr/include/c++/4.2.1/cstring \
  /usr/include/c++/4.2.1/cstddef \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stddef.h \
  /usr/include/string.h /usr/include/strings.h \
  /usr/include/c++/4.2.1/cstdio /usr/include/c++/4.2.1/cstdarg \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  /usr/include/c++/4.2.1/bits/c++io.h /usr/include/c++/4.2.1/bits/gthr.h \
  /usr/include/c++/4.2.1/bits/gthr-default.h /usr/include/pthread.h \
  /usr/include/pthread_impl.h /usr/include/sched.h /usr/include/time.h \
  /usr/include/_structs.h /usr/include/c++/4.2.1/cctype \
  /usr/include/ctype.h /usr/include/runetype.h \
  /usr/include/c++/4.2.1/bits/stringfwd.h \
  /usr/include/c++/4.2.1/bits/postypes.h /usr/include/c++/4.2.1/cwchar \
  /usr/include/c++/4.2.1/ctime /usr/include/wchar.h \
  /usr/include/_wctype.h /usr/include/stdint.h \
  /usr/include/c++/4.2.1/bits/functexcept.h \
  /usr/include/c++/4.2.1/exception_defines.h \
  /usr/include/c++/4.2.1/exception \
  /usr/include/c++/4.2.1/bits/char_traits.h \
  /usr/include/c++/4.2.1/bits/stl_algobase.h \
  /usr/include/c++/4.2.1/climits \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /usr/include/limits.h /usr/include/machine/limits.h \
  /usr/include/i386/limits.h /usr/include/i386/_limits.h \
  /usr/include/sys/syslimits.h /usr/include/c++/4.2.1/cstdlib \
  /usr/include/c++/4.2.1/bits/stl_pair.h \
  /usr/include/c++/4.2.1/bits/cpp_type_traits.h \
  /usr/include/c++/4.2.1/ext/type_traits.h /usr/include/c++/4.2.1/utility \
  /usr/include/c++/4.2.1/bits/stl_relops.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.2.1/bits/concept_check.h \
  /usr/include/c++/4.2.1/bits/stl_iterator.h \
  /usr/include/c++/4.2.1/debug/debug.h \
  /usr/include/c++/4.2.1/bits/localefwd.h \
  /usr/include/c++/4.2.1/bits/ios_base.h \
  /usr/include/c++/4.2.1/ext/atomicity.h \
  /usr/include/c++/4.2.1/bits/atomic_word.h \
  /usr/include/c++/4.2.1/bits/locale_classes.h \
  /usr/include/c++/4.2.1/string /usr/include/c++/4.2.1/memory \
  /usr/include/c++/4.2.1/bits/allocator.h \
  /usr/include/c++/4.2.1/bits/c++allocator.h \
  /usr/include/c++/4.2.1/ext/new_allocator.h /usr/include/c++/4.2.1/new \
  /usr/include/c++/4.2.1/bits/stl_construct.h \
  /usr/include/c++/4.2.1/bits/stl_uninitialized.h \
  /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h \
  /usr/include/c++/4.2.1/limits \
  /usr/include/c++/4.2.1/bits/ostream_insert.h \
  /usr/include/c++/4.2.1/bits/stl_function.h \
  /usr/include/c++/4.2.1/bits/basic_string.h \
  /usr/include/c++/4.2.1/algorithm /usr/include/c++/4.2.1/bits/stl_algo.h \
  /usr/include/c++/4.2.1/bits/stl_heap.h \
  /usr/include/c++/4.2.1/bits/stl_tempbuf.h \
  /usr/include/c++/4.2.1/bits/basic_string.tcc \
  /usr/include/c++/4.2.1/streambuf \
  /usr/include/c++/4.2.1/bits/streambuf.tcc \
  /usr/include/c++/4.2.1/bits/basic_ios.h \
  /usr/include/c++/4.2.1/bits/streambuf_iterator.h \
  /usr/include/c++/4.2.1/bits/locale_facets.h \
  /usr/include/c++/4.2.1/cwctype /usr/include/wctype.h \
  /usr/include/c++/4.2.1/bits/ctype_base.h \
  /usr/include/c++/4.2.1/bits/ctype_inline.h \
  /usr/include/c++/4.2.1/bits/codecvt.h \
  /usr/include/c++/4.2.1/bits/time_members.h \
  /usr/include/c++/4.2.1/bits/messages_members.h \
  /usr/include/c++/4.2.1/bits/basic_ios.tcc \
  /usr/include/c++/4.2.1/bits/ostream.tcc /usr/include/c++/4.2.1/locale \
  /usr/include/c++/4.2.1/bits/locale_facets.tcc \
  /usr/include/c++/4.2.1/typeinfo /usr/include/c++/4.2.1/istream \
  /usr/include/c++/4.2.1/bits/istream.tcc Descriptors.h \
  /usr/include/c++/4.2.1/map /usr/include/c++/4.2.1/bits/stl_tree.h \
  /usr/include/c++/4.2.1/bits/stl_map.h \
  /usr/include/c++/4.2.1/bits/stl_multimap.h TSTypes.h Logger.h \
  BitStream.h ByteArrayBuffer.h ByteArray.h TimeDateSection.h Table.h \
  MJD.h
TransportPacket.o: TransportPacket.cc /usr/include/assert.h \
  /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/stdlib.h \
  /usr/include/Availability.h /usr/include/AvailabilityInternal.h \
  /usr/include/_types.h /usr/include/sys/_types.h \
  /usr/include/machine/_types.h /usr/include/i386/_types.h \
  /usr/include/sys/wait.h /usr/include/sys/signal.h \
  /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h \
  /usr/include/i386/signal.h /usr/include/i386/_structs.h \
  /usr/include/sys/_structs.h /usr/include/machine/_structs.h \
  /usr/include/mach/i386/_structs.h /usr/include/sys/resource.h \
  /usr/include/machine/endian.h /usr/include/i386/endian.h \
  /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/i386/_types.h TransportPacket.h \
  /usr/include/c++/4.2.1/iostream /usr/include/c++/4.2.1/bits/c++config.h \
  /usr/include/c++/4.2.1/bits/os_defines.h /usr/include/unistd.h \
  /usr/include/sys/unistd.h /usr/include/sys/select.h \
  /usr/include/sys/_select.h /usr/include/c++/4.2.1/bits/cpu_defines.h \
  /usr/include/c++/4.2.1/ostream /usr/include/c++/4.2.1/ios \
  /usr/include/c++/4.2.1/iosfwd /usr/include/c++/4.2.1/bits/c++locale.h \
  /usr/include/c++/4.2.1/clocale /usr/include/locale.h \
  /usr/include/_locale.h /usr/include/c++/4.2.1/cstring \
  /usr/include/c++/4.2.1/cstddef \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stddef.h \
  /usr/include/string.h /usr/include/strings.h \
  /usr/include/c++/4.2.1/cstdio /usr/include/stdio.h \
  /usr/include/c++/4.2.1/cstdarg \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  /usr/include/c++/4.2.1/bits/c++io.h /usr/include/c++/4.2.1/bits/gthr.h \
  /usr/include/c++/4.2.1/bits/gthr-default.h /usr/include/pthread.h \
  /usr/include/pthread_impl.h /usr/include/sched.h /usr/include/time.h \
  /usr/include/_structs.h /usr/include/c++/4.2.1/cctype \
  /usr/include/ctype.h /usr/include/runetype.h \
  /usr/include/c++/4.2.1/bits/stringfwd.h \
  /usr/include/c++/4.2.1/bits/postypes.h /usr/include/c++/4.2.1/cwchar \
  /usr/include/c++/4.2.1/ctime /usr/include/wchar.h \
  /usr/include/_wctype.h /usr/include/stdint.h \
  /usr/include/c++/4.2.1/bits/functexcept.h \
  /usr/include/c++/4.2.1/exception_defines.h \
  /usr/include/c++/4.2.1/exception \
  /usr/include/c++/4.2.1/bits/char_traits.h \
  /usr/include/c++/4.2.1/bits/stl_algobase.h \
  /usr/include/c++/4.2.1/climits \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /usr/include/limits.h /usr/include/machine/limits.h \
  /usr/include/i386/limits.h /usr/include/i386/_limits.h \
  /usr/include/sys/syslimits.h /usr/include/c++/4.2.1/cstdlib \
  /usr/include/c++/4.2.1/bits/stl_pair.h \
  /usr/include/c++/4.2.1/bits/cpp_type_traits.h \
  /usr/include/c++/4.2.1/ext/type_traits.h /usr/include/c++/4.2.1/utility \
  /usr/include/c++/4.2.1/bits/stl_relops.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.2.1/bits/concept_check.h \
  /usr/include/c++/4.2.1/bits/stl_iterator.h \
  /usr/include/c++/4.2.1/debug/debug.h \
  /usr/include/c++/4.2.1/bits/localefwd.h \
  /usr/include/c++/4.2.1/bits/ios_base.h \
  /usr/include/c++/4.2.1/ext/atomicity.h \
  /usr/include/c++/4.2.1/bits/atomic_word.h \
  /usr/include/c++/4.2.1/bits/locale_classes.h \
  /usr/include/c++/4.2.1/string /usr/include/c++/4.2.1/memory \
  /usr/include/c++/4.2.1/bits/allocator.h \
  /usr/include/c++/4.2.1/bits/c++allocator.h \
  /usr/include/c++/4.2.1/ext/new_allocator.h /usr/include/c++/4.2.1/new \
  /usr/include/c++/4.2.1/bits/stl_construct.h \
  /usr/include/c++/4.2.1/bits/stl_uninitialized.h \
  /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h \
  /usr/include/c++/4.2.1/limits \
  /usr/include/c++/4.2.1/bits/ostream_insert.h \
  /usr/include/c++/4.2.1/bits/stl_function.h \
  /usr/include/c++/4.2.1/bits/basic_string.h \
  /usr/include/c++/4.2.1/algorithm /usr/include/c++/4.2.1/bits/stl_algo.h \
  /usr/include/c++/4.2.1/bits/stl_heap.h \
  /usr/include/c++/4.2.1/bits/stl_tempbuf.h \
  /usr/include/c++/4.2.1/bits/basic_string.tcc \
  /usr/include/c++/4.2.1/streambuf \
  /usr/include/c++/4.2.1/bits/streambuf.tcc \
  /usr/include/c++/4.2.1/bits/basic_ios.h \
  /usr/include/c++/4.2.1/bits/streambuf_iterator.h \
  /usr/include/c++/4.2.1/bits/locale_facets.h \
  /usr/include/c++/4.2.1/cwctype /usr/include/wctype.h \
  /usr/include/c++/4.2.1/bits/ctype_base.h \
  /usr/include/c++/4.2.1/bits/ctype_inline.h \
  /usr/include/c++/4.2.1/bits/codecvt.h \
  /usr/include/c++/4.2.1/bits/time_members.h \
  /usr/include/c++/4.2.1/bits/messages_members.h \
  /usr/include/c++/4.2.1/bits/basic_ios.tcc \
  /usr/include/c++/4.2.1/bits/ostream.tcc /usr/include/c++/4.2.1/locale \
  /usr/include/c++/4.2.1/bits/locale_facets.tcc \
  /usr/include/c++/4.2.1/typeinfo /usr/include/c++/4.2.1/istream \
  /usr/include/c++/4.2.1/bits/istream.tcc TSTypes.h Logger.h \
  ByteArrayBuffer.h ByteArray.h BitStream.h AdaptationField.h \
  ProgramMapSection.h Table.h ProgramAssociationSection.h
TransportStream.o: TransportStream.cc /usr/include/stdio.h \
  /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/Availability.h \
  /usr/include/AvailabilityInternal.h /usr/include/_types.h \
  /usr/include/sys/_types.h /usr/include/machine/_types.h \
  /usr/include/i386/_types.h /usr/include/assert.h /usr/include/stdlib.h \
  /usr/include/sys/wait.h /usr/include/sys/signal.h \
  /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h \
  /usr/include/i386/signal.h /usr/include/i386/_structs.h \
  /usr/include/sys/_structs.h /usr/include/machine/_structs.h \
  /usr/include/mach/i386/_structs.h /usr/include/sys/resource.h \
  /usr/include/machine/endian.h /usr/include/i386/endian.h \
  /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/i386/_types.h TransportStream.h \
  /usr/include/c++/4.2.1/iostream /usr/include/c++/4.2.1/bits/c++config.h \
  /usr/include/c++/4.2.1/bits/os_defines.h /usr/include/unistd.h \
  /usr/include/sys/unistd.h /usr/include/sys/select.h \
  /usr/include/sys/_select.h /usr/include/c++/4.2.1/bits/cpu_defines.h \
  /usr/include/c++/4.2.1/ostream /usr/include/c++/4.2.1/ios \
  /usr/include/c++/4.2.1/iosfwd /usr/include/c++/4.2.1/bits/c++locale.h \
  /usr/include/c++/4.2.1/clocale /usr/include/locale.h \
  /usr/include/_locale.h /usr/include/c++/4.2.1/cstring \
  /usr/include/c++/4.2.1/cstddef \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stddef.h \
  /usr/include/string.h /usr/include/strings.h \
  /usr/include/c++/4.2.1/cstdio /usr/include/c++/4.2.1/cstdarg \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  /usr/include/c++/4.2.1/bits/c++io.h /usr/include/c++/4.2.1/bits/gthr.h \
  /usr/include/c++/4.2.1/bits/gthr-default.h /usr/include/pthread.h \
  /usr/include/pthread_impl.h /usr/include/sched.h /usr/include/time.h \
  /usr/include/_structs.h /usr/include/c++/4.2.1/cctype \
  /usr/include/ctype.h /usr/include/runetype.h \
  /usr/include/c++/4.2.1/bits/stringfwd.h \
  /usr/include/c++/4.2.1/bits/postypes.h /usr/include/c++/4.2.1/cwchar \
  /usr/include/c++/4.2.1/ctime /usr/include/wchar.h \
  /usr/include/_wctype.h /usr/include/stdint.h \
  /usr/include/c++/4.2.1/bits/functexcept.h \
  /usr/include/c++/4.2.1/exception_defines.h \
  /usr/include/c++/4.2.1/exception \
  /usr/include/c++/4.2.1/bits/char_traits.h \
  /usr/include/c++/4.2.1/bits/stl_algobase.h \
  /usr/include/c++/4.2.1/climits \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /usr/include/limits.h /usr/include/machine/limits.h \
  /usr/include/i386/limits.h /usr/include/i386/_limits.h \
  /usr/include/sys/syslimits.h /usr/include/c++/4.2.1/cstdlib \
  /usr/include/c++/4.2.1/bits/stl_pair.h \
  /usr/include/c++/4.2.1/bits/cpp_type_traits.h \
  /usr/include/c++/4.2.1/ext/type_traits.h /usr/include/c++/4.2.1/utility \
  /usr/include/c++/4.2.1/bits/stl_relops.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.2.1/bits/concept_check.h \
  /usr/include/c++/4.2.1/bits/stl_iterator.h \
  /usr/include/c++/4.2.1/debug/debug.h \
  /usr/include/c++/4.2.1/bits/localefwd.h \
  /usr/include/c++/4.2.1/bits/ios_base.h \
  /usr/include/c++/4.2.1/ext/atomicity.h \
  /usr/include/c++/4.2.1/bits/atomic_word.h \
  /usr/include/c++/4.2.1/bits/locale_classes.h \
  /usr/include/c++/4.2.1/string /usr/include/c++/4.2.1/memory \
  /usr/include/c++/4.2.1/bits/allocator.h \
  /usr/include/c++/4.2.1/bits/c++allocator.h \
  /usr/include/c++/4.2.1/ext/new_allocator.h /usr/include/c++/4.2.1/new \
  /usr/include/c++/4.2.1/bits/stl_construct.h \
  /usr/include/c++/4.2.1/bits/stl_uninitialized.h \
  /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h \
  /usr/include/c++/4.2.1/limits \
  /usr/include/c++/4.2.1/bits/ostream_insert.h \
  /usr/include/c++/4.2.1/bits/stl_function.h \
  /usr/include/c++/4.2.1/bits/basic_string.h \
  /usr/include/c++/4.2.1/algorithm /usr/include/c++/4.2.1/bits/stl_algo.h \
  /usr/include/c++/4.2.1/bits/stl_heap.h \
  /usr/include/c++/4.2.1/bits/stl_tempbuf.h \
  /usr/include/c++/4.2.1/bits/basic_string.tcc \
  /usr/include/c++/4.2.1/streambuf \
  /usr/include/c++/4.2.1/bits/streambuf.tcc \
  /usr/include/c++/4.2.1/bits/basic_ios.h \
  /usr/include/c++/4.2.1/bits/streambuf_iterator.h \
  /usr/include/c++/4.2.1/bits/locale_facets.h \
  /usr/include/c++/4.2.1/cwctype /usr/include/wctype.h \
  /usr/include/c++/4.2.1/bits/ctype_base.h \
  /usr/include/c++/4.2.1/bits/ctype_inline.h \
  /usr/include/c++/4.2.1/bits/codecvt.h \
  /usr/include/c++/4.2.1/bits/time_members.h \
  /usr/include/c++/4.2.1/bits/messages_members.h \
  /usr/include/c++/4.2.1/bits/basic_ios.tcc \
  /usr/include/c++/4.2.1/bits/ostream.tcc /usr/include/c++/4.2.1/locale \
  /usr/include/c++/4.2.1/bits/locale_facets.tcc \
  /usr/include/c++/4.2.1/typeinfo /usr/include/c++/4.2.1/istream \
  /usr/include/c++/4.2.1/bits/istream.tcc /usr/include/c++/4.2.1/vector \
  /usr/include/c++/4.2.1/bits/stl_vector.h \
  /usr/include/c++/4.2.1/bits/stl_bvector.h \
  /usr/include/c++/4.2.1/bits/vector.tcc /usr/include/c++/4.2.1/map \
  /usr/include/c++/4.2.1/bits/stl_tree.h \
  /usr/include/c++/4.2.1/bits/stl_map.h \
  /usr/include/c++/4.2.1/bits/stl_multimap.h TSTypes.h Logger.h Table.h \
  BitStream.h ByteArrayBuffer.h ByteArray.h TransportPacket.h \
  ProgramAssociationSection.h ProgramMapSection.h EventInformationTable.h \
  MJD.h ServiceDescriptionTable.h TimeDateSection.h
ts_dump.o: ts_dump.cc /usr/include/c++/4.2.1/iostream \
  /usr/include/c++/4.2.1/bits/c++config.h \
  /usr/include/c++/4.2.1/bits/os_defines.h /usr/include/unistd.h \
  /usr/include/_types.h /usr/include/sys/_types.h \
  /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h \
  /usr/include/sys/_posix_availability.h /usr/include/machine/_types.h \
  /usr/include/i386/_types.h /usr/include/sys/unistd.h \
  /usr/include/Availability.h /usr/include/AvailabilityInternal.h \
  /usr/include/sys/select.h /usr/include/sys/appleapiopts.h \
  /usr/include/sys/_structs.h /usr/include/sys/_select.h \
  /usr/include/c++/4.2.1/bits/cpu_defines.h \
  /usr/include/c++/4.2.1/ostream /usr/include/c++/4.2.1/ios \
  /usr/include/c++/4.2.1/iosfwd /usr/include/c++/4.2.1/bits/c++locale.h \
  /usr/include/c++/4.2.1/clocale /usr/include/locale.h \
  /usr/include/_locale.h /usr/include/c++/4.2.1/cstring \
  /usr/include/c++/4.2.1/cstddef \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stddef.h \
  /usr/include/string.h /usr/include/strings.h \
  /usr/include/c++/4.2.1/cstdio /usr/include/stdio.h \
  /usr/include/c++/4.2.1/cstdarg \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/stdarg.h \
  /usr/include/c++/4.2.1/bits/c++io.h /usr/include/c++/4.2.1/bits/gthr.h \
  /usr/include/c++/4.2.1/bits/gthr-default.h /usr/include/pthread.h \
  /usr/include/pthread_impl.h /usr/include/sched.h /usr/include/time.h \
  /usr/include/_structs.h /usr/include/c++/4.2.1/cctype \
  /usr/include/ctype.h /usr/include/runetype.h \
  /usr/include/c++/4.2.1/bits/stringfwd.h \
  /usr/include/c++/4.2.1/bits/postypes.h /usr/include/c++/4.2.1/cwchar \
  /usr/include/c++/4.2.1/ctime /usr/include/wchar.h \
  /usr/include/_wctype.h /usr/include/stdint.h \
  /usr/include/c++/4.2.1/bits/functexcept.h \
  /usr/include/c++/4.2.1/exception_defines.h \
  /usr/include/c++/4.2.1/exception \
  /usr/include/c++/4.2.1/bits/char_traits.h \
  /usr/include/c++/4.2.1/bits/stl_algobase.h \
  /usr/include/c++/4.2.1/climits \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /usr/llvm-gcc-4.2/bin/../lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/limits.h \
  /Developer/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include/syslimits.h \
  /usr/include/limits.h /usr/include/machine/limits.h \
  /usr/include/i386/limits.h /usr/include/i386/_limits.h \
  /usr/include/sys/syslimits.h /usr/include/c++/4.2.1/cstdlib \
  /usr/include/stdlib.h /usr/include/sys/wait.h /usr/include/sys/signal.h \
  /usr/include/machine/signal.h /usr/include/i386/signal.h \
  /usr/include/i386/_structs.h /usr/include/machine/_structs.h \
  /usr/include/mach/i386/_structs.h /usr/include/sys/resource.h \
  /usr/include/machine/endian.h /usr/include/i386/endian.h \
  /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/i386/_types.h /usr/include/c++/4.2.1/bits/stl_pair.h \
  /usr/include/c++/4.2.1/bits/cpp_type_traits.h \
  /usr/include/c++/4.2.1/ext/type_traits.h /usr/include/c++/4.2.1/utility \
  /usr/include/c++/4.2.1/bits/stl_relops.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h \
  /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h \
  /usr/include/c++/4.2.1/bits/concept_check.h \
  /usr/include/c++/4.2.1/bits/stl_iterator.h \
  /usr/include/c++/4.2.1/debug/debug.h \
  /usr/include/c++/4.2.1/bits/localefwd.h \
  /usr/include/c++/4.2.1/bits/ios_base.h \
  /usr/include/c++/4.2.1/ext/atomicity.h \
  /usr/include/c++/4.2.1/bits/atomic_word.h \
  /usr/include/c++/4.2.1/bits/locale_classes.h \
  /usr/include/c++/4.2.1/string /usr/include/c++/4.2.1/memory \
  /usr/include/c++/4.2.1/bits/allocator.h \
  /usr/include/c++/4.2.1/bits/c++allocator.h \
  /usr/include/c++/4.2.1/ext/new_allocator.h /usr/include/c++/4.2.1/new \
  /usr/include/c++/4.2.1/bits/stl_construct.h \
  /usr/include/c++/4.2.1/bits/stl_uninitialized.h \
  /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h \
  /usr/include/c++/4.2.1/limits \
  /usr/include/c++/4.2.1/bits/ostream_insert.h \
  /usr/include/c++/4.2.1/bits/stl_function.h \
  /usr/include/c++/4.2.1/bits/basic_string.h \
  /usr/include/c++/4.2.1/algorithm /usr/include/c++/4.2.1/bits/stl_algo.h \
  /usr/include/c++/4.2.1/bits/stl_heap.h \
  /usr/include/c++/4.2.1/bits/stl_tempbuf.h \
  /usr/include/c++/4.2.1/bits/basic_string.tcc \
  /usr/include/c++/4.2.1/streambuf \
  /usr/include/c++/4.2.1/bits/streambuf.tcc \
  /usr/include/c++/4.2.1/bits/basic_ios.h \
  /usr/include/c++/4.2.1/bits/streambuf_iterator.h \
  /usr/include/c++/4.2.1/bits/locale_facets.h \
  /usr/include/c++/4.2.1/cwctype /usr/include/wctype.h \
  /usr/include/c++/4.2.1/bits/ctype_base.h \
  /usr/include/c++/4.2.1/bits/ctype_inline.h \
  /usr/include/c++/4.2.1/bits/codecvt.h \
  /usr/include/c++/4.2.1/bits/time_members.h \
  /usr/include/c++/4.2.1/bits/messages_members.h \
  /usr/include/c++/4.2.1/bits/basic_ios.tcc \
  /usr/include/c++/4.2.1/bits/ostream.tcc /usr/include/c++/4.2.1/locale \
  /usr/include/c++/4.2.1/bits/locale_facets.tcc \
  /usr/include/c++/4.2.1/typeinfo /usr/include/c++/4.2.1/istream \
  /usr/include/c++/4.2.1/bits/istream.tcc /usr/include/c++/4.2.1/fstream \
  /usr/include/c++/4.2.1/bits/basic_file.h \
  /usr/include/c++/4.2.1/bits/fstream.tcc /usr/include/getopt.h \
  TransportStream.h /usr/include/c++/4.2.1/vector \
  /usr/include/c++/4.2.1/bits/stl_vector.h \
  /usr/include/c++/4.2.1/bits/stl_bvector.h \
  /usr/include/c++/4.2.1/bits/vector.tcc /usr/include/c++/4.2.1/map \
  /usr/include/c++/4.2.1/bits/stl_tree.h \
  /usr/include/c++/4.2.1/bits/stl_map.h \
  /usr/include/c++/4.2.1/bits/stl_multimap.h TSTypes.h Logger.h Table.h \
  BitStream.h ByteArrayBuffer.h ByteArray.h /usr/include/assert.h \
  TransportPacket.h ProgramAssociationSection.h ProgramMapSection.h \
  EventInformationTable.h MJD.h StdLogger.h
