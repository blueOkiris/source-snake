# Build properties
OBJNAME := 	snake
CPPC :=		g++
CPPFLAGS :=	-Wall -O2 -std=c++17
LD :=		g++
LDFLAGS :=	-Llink -linp -lres -lloop \
			-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SRC :=		$(wildcard src/*.cpp)
HFILES :=	$(wildcard include/*.hpp)
INC := 		-Iinclude -Ilink/include/inp -Ilink/include/loop -Ilink/include/res
LINK_LIB :=	link

# Autogen vars
OBJS := 	$(subst .cpp,.o,$(subst src/,obj/,$(SRC)))

# Helper targets
.PHONY : all
all : $(OBJNAME)

.PHONY : install-deps
install-deps :
	sudo apt install -y build-essential make libsfml-dev

.PHONY : clean
clean :
	rm -rf obj/
	rm -rf $(OBJNAME)
	rm -rf .highscore.txt
	rm -rf $(LINK_LIB)
	rm -rf *.xz

obj/%.o : src/%.cpp $(HFILES) $(LINK_LIB)
	mkdir -p obj
	$(CPPC) -o $@ $(CPPFLAGS) $(INC) -c $<

# Main targets
$(LINK_LIB) :
	wget https://github.com/blueOkiris/link/releases/download/v21.414.1500/link-v21.414.1500.tar.xz
	mkdir -p link
	tar xf link-v21.414.1500.tar.xz -C link

$(OBJNAME) : $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)
