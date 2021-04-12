# Build properties
OBJNAME := 	snake
CPPC :=		g++
CPPFLAGS :=	-Wall -O2 -std=c++17
LD :=		g++
LDFLAGS :=	-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SRC :=		$(wildcard src/*.cpp)
HFILES :=	$(wildcard include/*.hpp)
INC := 		-Iinclude

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

obj/%.o : src/%.cpp $(HFILES)
	mkdir -p obj
	$(CPPC) -o $@ $(CPPFLAGS) $(INC) -c $<

# Main targets
$(OBJNAME) : $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)
