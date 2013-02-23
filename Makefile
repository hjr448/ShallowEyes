# manual Makefile

PROJECT = shallow_eye
<<<<<<< HEAD
CC = gcc

OBJ_DIR = ./tmp
OUTPUT_DIR = .
TARGET = schick401.exe
C_INCLUDE_DIRS = -I"../irrlicht-1.8/include"
=======
CC = "C:\MinGW\bin\g++.exe"


OBJ_DIR = "./tmp"
OUTPUT_DIR = .
TARGET = schick401.exe
C_INCLUDE_DIRS = -I"../irrlicht-1.8/include" 
>>>>>>> master
C_PREPROC = 
CFLAGS = -pipe  -g2 -O0 -frtti -fexceptions 
RC_INCLUDE_DIRS = 
RC_PREPROC = 
RCFLAGS = 
<<<<<<< HEAD
LIB_DIRS = -L"../irrlicht-1.8/lib/Win32-gcc" -L"../irrlicht-1.8/lib" -L/usr/local/lib
LIBS = -lirrlicht -lstdc++ 
=======
LIB_DIRS = -L"../irrlicht-1.8/lib/Win32-gcc" -L"../irrlicht-1.8/lib" 
LIBS = -lirrlicht 
>>>>>>> master
LDFLAGS = -pipe 

SRC_OBJS = \
  $(OBJ_DIR)/sm401_main.o	\
  $(OBJ_DIR)/sm401_seg002.o	\
  $(OBJ_DIR)/sm401_video.o	\
  $(OBJ_DIR)/sm401_seg009.o	\
  $(OBJ_DIR)/sm401_GUI.o	\
<<<<<<< HEAD
  $(OBJ_DIR)/gen201.o		\
  $(OBJ_DIR)/CVoc.o
=======
  $(OBJ_DIR)/gen201.o
>>>>>>> master

define build_target
@echo Linking...
@$(CC) -o "$(OUTPUT_DIR)\$(TARGET)" $(SRC_OBJS) $(LIB_DIRS) $(LIBS) $(LDFLAGS)
endef

define compile_source
@echo Compiling $<
@$(CC) $(CFLAGS) $(C_PREPROC) $(C_INCLUDE_DIRS) -c "$<" -o "$@"
endef

.PHONY: print_header directories

$(TARGET): print_header directories $(SRC_OBJS)
	$(build_target)

.PHONY: clean cleanall

cleanall:
<<<<<<< HEAD
	@echo Deleting intermediate files for 'shallow_eye'
	-@rm $(OBJ_DIR)/*.o
	-@rm "$(OUTPUT_DIR)/$(TARGET)"

clean:
	@echo Deleting intermediate files for 'shallow_eye'
	-@rm $(OBJ_DIR)/*.o

print_header:
	@echo ----------Configuration: shallow_eye -----------

directories:
	-@if test ! -d $(OUTPUT_DIR); then mkdir $(OUTPUT_DIR); else : ; fi;
	-@if test ! -d $(OBJ_DIR); then mkdir $(OBJ_DIR); else : ; fi;
=======
	@echo Deleting intermediate files for 'shallow_eye - $(CFG)'
	-@del $(OBJ_DIR)\*.o
	-@del "$(OUTPUT_DIR)\$(TARGET)"
	-@rmdir "$(OUTPUT_DIR)"

clean:
	@echo Deleting intermediate files for 'shallow_eye - $(CFG)'
	-@del $(OBJ_DIR)\*.o

print_header:
	@echo ----------Configuration: shallow_eye - $(CFG)----------

directories:
	-@if not exist "$(OUTPUT_DIR)\$(NULL)" mkdir "$(OUTPUT_DIR)"
	-@if not exist "$(OBJ_DIR)\$(NULL)" mkdir "$(OBJ_DIR)"
>>>>>>> master

$(OBJ_DIR)/sm401_main.o: sm401_main.cpp	\
sm401_main.h	\
sm401_seg002.h	\
sm401_seg009.h	\
sm401_GUI.h	\
sm401_video.h	\
sm401_vars.h	\
gen201.h
	$(compile_source)

$(OBJ_DIR)/sm401_seg002.o: sm401_seg002.cpp	\
sm401_main.h	\
sm401_seg002.h	\
sm401_seg009.h
	$(compile_source)

$(OBJ_DIR)/sm401_video.o: sm401_video.cpp	\
sm401_main.h	\
sm401_seg002.h	\
sm401_GUI.h	\
sm401_video.h
	$(compile_source)

$(OBJ_DIR)/sm401_seg009.o: sm401_seg009.cpp	\
sm401_main.h	\
sm401_seg002.h	\
sm401_seg009.h
	$(compile_source)

$(OBJ_DIR)/sm401_GUI.o: sm401_GUI.cpp	\
sm401_main.h	\
sm401_seg002.h	\
sm401_GUI.h	\
sm401_video.h
	$(compile_source)

$(OBJ_DIR)/gen201.o: gen201.cpp	\
sm401_main.h	\
sm401_GUI.h	\
sm401_video.h	\
gen201.h	\
sm401_seg002.h	\
sm401_seg009.h	\
hero.h	\
gen201_vars.h
	$(compile_source)

<<<<<<< HEAD
$(OBJ_DIR)/CVoc.o: CVoc.cpp	\
CVoc.h
	$(compile_source)


=======
>>>>>>> master
