GRADLE=./gradlew
BUILD_TASK=shadowJar
CLEAN_TASK=clean
BUILD_DIR=build/libs/
ARTIFACT_NAME=Rogue-1.0.jar
SO_PATH=temp/jcurses/lib
SO_NAME=libjcurses.so
NCURSES_NAME=libncurses.so.6.3
NCURSES_PATH=/usr/lib/x86_64-linux-gnu/
#defauld: run
all: build run
clean:
	$(GRADLE) $(CLEAN_TASK)
	rm -rf temp
run: build
	java -jar $(BUILD_DIR)$(ARTIFACT_NAME)
build: clean
	$(GRADLE) build
#	./gradlew build
lib_install:
	rm -rf temp
	mkdir temp
	cd ./temp && wget -O jcurses-linux-0.9.5b.tar.gz https://sourceforge.net/projects/javacurses/files/javacurses/0.9.5b/jcurses-linux-0.9.5b.tar.gz/download && tar -xf jcurses-linux-0.9.5b.tar.gz
	rm $(SO_PATH)/$(SO_NAME)
	mv $(SO_PATH)/libjcurses64.so $(SO_PATH)/$(SO_NAME)
	mkdir -p /usr/java/packages/lib
	cp $(SO_PATH)/$(SO_NAME) /usr/java/packages/lib/$(SO_NAME)
	ln -s $(NCURSES_PATH)$(NCURSES_NAME) $(NCURSES_PATH)libncurses.so.5
	rm -rf temp