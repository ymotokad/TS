#DO NOT EDIT TAHIS FILE. EDIT Makefile.tmpl INSTEAD.
CXXFLAGS=-O2 -DNDEBUG
#CXXFLAGS=-g -O2
LIB_ICONV=-L/opt/local/lib -liconv
LIB_ICONV=

OBJS=StdLogger.o \
	ByteArray.o ByteArrayBuffer.o BitStream.o \
	ISO13818_SystemClock.o \
	ISO13818_Table.o \
	ISO13818_ProgramAssociationSection.o \
	ISO13818_ProgramMapSection.o \
	ISO13818_AdaptationField.o \
	ISO13818_PacketizedElementaryStream.o \
	ISO13818_MPEGHeaders.o \
	ISO13818_MPEGStream.o \
	B10_Descriptors.o \
	B10_MJD.o \
	B10_ServiceDescriptionTable.o \
	B10_EventInformationTable.o \
	B10_TimeDateSection.o \
	ISO13818_TransportPacket.o \
	ISO13818_TransportStream.o

all: tsfilter tsinfo tsfilter_lite ts2ass

tsfilter: $(OBJS) tsfilter.o
	$(CXX) -o $@ $(OBJS) tsfilter.o $(LIBS)

tsfilter_lite: $(OBJS) tsfilter_lite.o
	$(CXX) -o $@ $(OBJS) tsfilter_lite.o $(LIBS)

tsinfo: $(OBJS) tsinfo.o
	$(CXX) -o $@ $(OBJS) tsinfo.o $(LIBS)

ts2ass: $(OBJS) ts2ass.o B24_Caption.o
	$(CXX) -o $@ $(OBJS) ts2ass.o B24_Caption.o $(LIBS) $(LIB_ICONV)

clean:
	@rm -f $(OBJS) *.o *~ tsfilter tsinfo tsfilter_lite ts2ass
	@echo "#DO NOT EDIT TAHIS FILE. EDIT Makefile.tmpl INSTEAD." > Makefile.tmp
	@cat Makefile.tmpl >> Makefile.tmp
	@rm -f Makefile
	@mv -f Makefile.tmp Makefile

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

install: all
	install tsfilter tsinfo ts2ass /usr/local/bin

depend:
	@echo "#DO NOT EDIT TAHIS FILE. EDIT Makefile.tmpl INSTEAD." > Makefile.tmp
	@cat Makefile.tmpl >> Makefile.tmp
	$(CXX) -M $(CXXFLAGS) *.cc >>Makefile.tmp
	@rm -f Makefile
	@mv -f Makefile.tmp Makefile
	
