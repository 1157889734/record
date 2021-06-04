
#CC = gcc

CC = arm-linux-gcc

#CC = arm-poky-linux-gnueabi-gcc  

CFLAGS  = -O2 -c -Wall -Imycode -Irecordmp3 


recorder: 		obj/main.o obj/MyApp.o obj/MyMiscFunc.o obj/MyNet.o obj/MyUdp.o	\
				obj/MyProtocol.o obj/MyCommander.o obj/MyRecord.o obj/MyTimer.o \
				obj/MyEncode.o obj/MyBoard.o obj/MyStorage.o obj/MyHttpd.o \
				obj/bitstream.o obj/coder.o obj/huffman.o obj/layer3.o obj/loop.o \
				obj/utils.o
				$(CC) -o $@ $^ -lpthread
				
				
obj/main.o:			mycode/main.c
					$(CC) $(CFLAGS) $< -o $@

obj/MyApp.o:		mycode/MyApp.c
					$(CC) $(CFLAGS) $< -o $@
				
obj/MyMiscFunc.o:	mycode/MyMiscFunc.c
					$(CC) $(CFLAGS) $< -o $@
				
obj/MyNet.o:		mycode/MyNet.c
					$(CC) $(CFLAGS) $< -o $@
				
obj/MyUdp.o:		mycode/MyUdp.c
					$(CC) $(CFLAGS) $< -o $@
				
obj/MyProtocol.o:	mycode/MyProtocol.c
					$(CC) $(CFLAGS) $< -o $@
				
obj/MyCommander.o:	mycode/MyCommander.c
					$(CC) $(CFLAGS) $< -o $@
					
obj/MyRecord.o:		mycode/MyRecord.c
					$(CC) $(CFLAGS) $< -o $@
					
obj/MyTimer.o:		mycode/MyTimer.c
					$(CC) $(CFLAGS) $< -o $@
					
obj/MyEncode.o:		mycode/MyEncode.c
					$(CC) $(CFLAGS) $< -o $@
					
obj/MyBoard.o:		mycode/MyBoard.c
					$(CC) $(CFLAGS) $< -o $@
					
obj/MyStorage.o:	mycode/MyStorage.c
					$(CC) $(CFLAGS) $< -o $@
					
obj/MyHttpd.o:		mycode/MyHttpd.c
					$(CC) $(CFLAGS) $< -o $@	
					
					
																	
obj/bitstream.o:	recordmp3/bitstream.c
					$(CC) $(CFLAGS) $< -o $@
					
obj/coder.o:		recordmp3/coder.c
					$(CC) $(CFLAGS) $< -o $@
					
obj/huffman.o:		recordmp3/huffman.c
					$(CC) $(CFLAGS) $< -o $@
					
obj/layer3.o:		recordmp3/layer3.c
					$(CC) $(CFLAGS) $< -o $@
					
obj/loop.o:			recordmp3/loop.c
					$(CC) $(CFLAGS) $< -o $@
					
obj/utils.o:		recordmp3/utils.c
					$(CC) $(CFLAGS) $< -o $@
							
				
	
clean:
	rm -f obj/*.o
	rm -f recorder