# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alelievr <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created  2014/07/15 15:13:38 by alelievr          #+#    #+#              #
#    Updated  2016/04/29 15:41:39 by alelievr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#################
##  VARIABLES  ##
#################

#	Sources
SRCDIR		=	sources
SRC			=	main.cpp			\
				Main.class.cpp		\
				Game.class.cpp		\
				Exception.class.cpp	\
				Timer.class.cpp		\

GLFWLIB_SRC	=	GUI/GLFW_gui.class.cpp	\

SDLLIB_SRC	=	GUI/SDL_gui.class.cpp	\

SFMLLIB_SRC	=	GUI/SFML_gui.class.cpp	\

SERVOTRON_SRC = servotron/servotron.class.cpp	\

#	Objects
OBJDIR		=	obj

#	Variables
LIBFT		=	2	#1 or 0 to include the libft / 2 for autodetct
DEBUGLEVEL	=	0	#can be 0 for no debug 1 for or 2 for harder debug
					#Warrning: non null debuglevel will disable optlevel
OPTLEVEL	=	1	#same than debuglevel
					#Warrning: non null optlevel will disable debuglevel
CPPVERSION	=	c++11
#For simpler and faster use, use commnd line variables DEBUG and OPTI:
#Example $> make DEBUG=2 will set debuglevel to 2

#	Includes
INCDIRS		=	sources sources/servotron

#	Libraries
LIBDIRS		=	
LDLIBS		=	-ldl

#	Output
NAME		=	nibbler

#	Compiler
WERROR		=	-Werror
CFLAGS		=	-Wall -Wextra -pedantic
CPROTECTION	=	-z execstack -fno-stack-protector

DEBUGFLAGS1	=	-ggdb -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -O0
DEBUGFLAGS2	=	#-fsanitize-memory-track-origins=2
OPTFLAGS1	=	-funroll-loops -O2
OPTFLAGS2	=	-pipe -funroll-loops -Ofast

#	Shared
SHAREDLIB_FLAGS = 	-shared -fPIC

#	Framework
FRAMEWORK		=

#################
##  COLORS     ##
#################
CPREFIX		=	"\033[38;5;"
BGPREFIX	=	"\033[48;5;"
CCLEAR		=	"\033[0m"
CLINK_T		=	$(CPREFIX)"129m"
CLINK		=	$(CPREFIX)"93m"
COBJ_T		=	$(CPREFIX)"119m"
COBJ		=	$(CPREFIX)"113m"
CCLEAN_T	=	$(CPREFIX)"9m"
CCLEAN		=	$(CPREFIX)"166m"
CRUN_T		=	$(CPREFIX)"198m"
CRUN		=	$(CPREFIX)"163m"
CNORM_T		=	"226m"
CNORM_ERR	=	"196m"
CNORM_WARN	=	"202m"
CNORM_OK	=	"231m"

#################
##  OS/PROC    ##
#################

OS			:=	$(shell uname -s)
PROC		:=	$(shell uname -p)
DEBUGFLAGS	=	""
LINKDEBUG	=	""
OPTFLAGS	=	""
#COMPILATION	=	""

ifeq "$(OS)" "Windows_NT"
endif
ifeq "$(OS)" "Linux"
	LDLIBS		+=
ifneq ($(filter clang,$(CC) $(CXX)),)
	DEBUGFLAGS	+= " "-fsanitize=memory -fsanitize=thread
endif
endif
ifeq "$(OS)" "Darwin"
FRAMEWORK	=	Cocoa OpenGL IOKit CoreVideo Carbon ForceFeedback AudioUnit CoreAudio
endif

#################
##  AUTO       ##
#################

NASM		=	nasm
OBJS		=	$(patsubst %.c,%.o, $(filter %.c, $(SRC))) \
				$(patsubst %.cpp,%.o, $(filter %.cpp, $(SRC))) \
				$(patsubst %.s,%.o, $(filter %.s, $(SRC)))
OBJ			=	$(addprefix $(OBJDIR)/,$(notdir $(OBJS)))
GLFWLIB_OBJ	=	$(addprefix $(OBJDIR)/,$(notdir $(GLFWLIB_SRC:.cpp=.o)))
SDLLIB_OBJ	=	$(addprefix $(OBJDIR)/,$(notdir $(SDLLIB_SRC:.cpp=.o)))
SFMLLIB_OBJ	=	$(addprefix $(OBJDIR)/,$(notdir $(SFMLLIB_SRC:.cpp=.o)))
NORME		=	**/*.[ch]
VPATH		+=	$(dir $(addprefix $(SRCDIR)/,$(SRC)))
VPATH		+=	$(dir $(addprefix $(SRCDIR)/,$(GLFWLIB_SRC)))
VPATH		+=	$(dir $(addprefix $(SRCDIR)/,$(SDLLIB_SRC)))
VPATH		+=	$(dir $(addprefix $(SRCDIR)/,$(SFMLLIB_SRC)))
VFRAME		=	$(addprefix -framework ,$(FRAMEWORK))
INCFILES	=	$(foreach inc, $(INCDIRS), $(wildcard $(inc)/*.h))
CPPFLAGS	=	$(addprefix -I,$(INCDIRS))
LDFLAGS		=	$(addprefix -L,$(LIBDIRS))
LINKER		=	$(CC)

disp_indent	=	for I in `seq 1 $(MAKELEVEL)`; do \
					test "$(MAKELEVEL)" '!=' '0' && printf "\t"; \
				done
color_exec	=	$(call disp_indent); \
				echo $(1)➤ $(3)$(2)"\n"$(strip $(4))$(CCLEAR);$(4)
color_exec_t=	$(call disp_indent); \
				echo $(1)➤ $(strip $(3))$(2)$(CCLEAR);$(3)

ifneq ($(filter 1,$(strip $(DEBUGLEVEL)) ${DEBUG}),)
	OPTLEVEL = 0
	OPTI = 0
	DEBUGFLAGS = $(DEBUGFLAGS1)
endif
ifneq ($(filter 2,$(strip $(DEBUGLEVEL)) ${DEBUG}),)
	OPTLEVEL = 0
	OPTI = 0
	DEBUGFLAGS = $(DEBUGFLAGS1)
	LINKDEBUG = $(DEBUGFLAGS1) $(DEBUGFLAGS2)
	export ASAN_OPTIONS=check_initialization_order=1
endif

ifneq ($(filter 1,$(strip $(OPTLEVEL)) ${OPTI}),)
	DEBUGFLAGS = ""
	OPTFLAGS = $(OPTFLAGS1)
endif
ifneq ($(filter 2,$(strip $(OPTLEVEL)) ${OPTI}),)
	DEBUGFLAGS = ""
	OPTFLAGS = $(OPTFLAGS1) $(OPTFLAGS2)
endif

ifndef $(CXX)
	CXX = c++
endif

ifneq ($(filter %.cpp,$(SRC)),)
	LINKER = $(CXX)
endif

ifdef ${NOWERROR}
	WERROR = ""
endif

ifeq "$(strip $(LIBFT))" "2"
ifneq ($(wildcard ./libft),)
	LIBDIRS += "libft"
	LDLIBS += "-lft"
	INCDIRS += "libft"
endif
endif

#################
##  TARGETS    ##
#################

SDLLIB = SDL2/build/.libs/libSDL2.a
SDLINCDIR = SDL2/include
SDLDIR_CHECK = SDL2/INSTALL.txt
SDL_NIBBLER_LIB = SDLnibbler.so
GLFWLIB = GLFW/src/libglfw3.a
GLFWDIR_CHECK = GLFW/README.md
GLFWINCDIR = GLFW/include
GLFW_NIBBLER_LIB = GLFWnibbler.so
SFMLLIB = SFML/SFML.framework
SFMLDIR_CHECK = SFML/readme.txt
SFMLINCDIR = SFML/include
SFML_NIBBLER_LIB = SFMLnibbler.so

SOILLIB = SOIL/lib/libSOIL.a
SOILDIR_CHECK = SOIL/README.md
SOILINCDIR = SOIL/src

CPPFLAGS += -I$(GLFWINCDIR) -I$(SDLINCDIR) -I$(SFMLINCDIR) -I$(SOILINCDIR)

#	First target
all: $(NAME) $(SDLLIB) $(GLFWLIB) $(SOILLIB) $(SFMLLIB) $(GLFW_NIBBLER_LIB) $(SDL_NIBBLER_LIB) $(SFML_NIBBLER_LIB)

$(SDLDIR_CHECK):
	@git submodule init
	@git submodule update

$(GLFWDIR_CHECK):
	@git submodule init
	@git submodule update

$(SFMLDIR_CHECK):
	@git submodule init
	@git submodule update

$(SOILDIR_CHECK):
	@git submodule init
	@git submodule update

$(SDLLIB): $(SDLDIR_CHECK)
	cd SDL2 && ./configure && make

$(SFMLLIB): $(SFMLDIR_CHECK)
	cd SFML && ./install.sh

$(GLFWLIB): $(GLFWDIR_CHECK)
	cd GLFW && cmake . && make

$(SOILLIB): $(SOILDIR_CHECK)
	cd SOIL && make

$(GLFW_NIBBLER_LIB): $(GLFWLIB_OBJ)
	@$(call color_exec,$(CLINK_T),$(CLINK),"Link of $(GLFW_NIBBLER_LIB):",\
		$(LINKER) $(SHAREDLIB_FLAGS) $(GLFWLIB) $(SOILLIB) $(OPTFLAGS)$(DEBUGFLAGS) $(VFRAME) -o $@ $(strip $^))

$(SDL_NIBBLER_LIB): $(SDLLIB_OBJ)
	@$(call color_exec,$(CLINK_T),$(CLINK),"Link of $(SDL_NIBBLER_LIB):",\
		$(LINKER) $(SHAREDLIB_FLAGS) $(SDLLIB) -liconv $(SOILLIB) $(OPTFLAGS)$(DEBUGFLAGS) $(VFRAME) -o $@ $(strip $^))

$(SFML_NIBBLER_LIB): $(SFMLLIB_OBJ)
	@export PATH=$PATH:./SFML/
	@$(call color_exec,$(CLINK_T),$(CLINK),"Link of $(SFML_NIBBLER_LIB):",\
		$(LINKER) $(SHAREDLIB_FLAGS) -framework SFML $(SOILLIB) $(OPTFLAGS)$(DEBUGFLAGS) $(VFRAME) -o $@ $(strip $^))

#	Linking
$(NAME): $(OBJ)
	@$(if $(findstring lft,$(LDLIBS)),$(call color_exec_t,$(CCLEAR),$(CCLEAR),\
		make -j 4 -C libft))
	@$(call color_exec,$(CLINK_T),$(CLINK),"Link of $(NAME):",\
		$(LINKER) $(LDFLAGS) $(LDLIBS) $(OPTFLAGS)$(DEBUGFLAGS)$(LINKDEBUG) -o $@ $(strip $^))

$(OBJDIR)/%.o: %.cpp $(INCFILES)
	@mkdir -p $(OBJDIR)
	@$(call color_exec,$(COBJ_T),$(COBJ),"Object: $@",\
		$(CXX) -std=$(CPPVERSION) $(WERROR) $(CFLAGS) $(OPTFLAGS)$(DEBUGFLAGS) $(CPPFLAGS) -o $@ -c $(strip $<))

#	Objects compilation
$(OBJDIR)/%.o: %.c $(INCFILES)
	@mkdir -p $(OBJDIR)
	@$(call color_exec,$(COBJ_T),$(COBJ),"Object: $@",\
		$(CC) $(WERROR) $(CFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) $(CPPFLAGS) -o $@ -c $<)

$(OBJDIR)/%.o: %.s
	@mkdir -p $(OBJDIR)
	@$(call color_exec,$(COBJ_T),$(COBJ),"Object: $@",\
		$(NASM) -f macho64 -o $@ $<)

#	Removing objects
clean:
	@$(call color_exec,$(CCLEAN_T),$(CCLEAN),"Clean:",\
		$(RM) -f $(OBJ))
	@rm -rf $(OBJDIR)
	@rm -rf GUI

#	Removing objects and exe
fclean: clean
	@$(call color_exec,$(CCLEAN_T),$(CCLEAN),"Fclean:",\
		$(RM) -f $(NAME) $(GLFW_NIBBLER_LIB) $(SDL_NIBBLER_LIB) $(SFML_NIBBLER_LIB))

#	All removing then compiling
re: fclean all

cleanall: fclean
	@$(call color_exec,$(CCLEAN_T),$(CCLEAN),"Fclean:",\
		$(RM) -f $(GLFWLIB) $(SDLLIB) $(SFMLLIB))

reall: cleanall all
 
f:	all run

#	Checking norme
norme:
	@norminette $(NORME) | sed "s/Norme/[38;5;$(CNORM_T)➤ [38;5;$(CNORM_OK)Norme/g;s/Warning/[0;$(CNORM_WARN)Warning/g;s/Error/[0;$(CNORM_ERR)Error/g"

run: $(NAME)
	@echo $(CRUN_T)"➤ "$(CRUN)"./$(NAME) ${ARGS}\033[0m"
	@./$(NAME) ${ARGS}

codesize:
	@cat $(NORME) |grep -v '/\*' |wc -l

functions: $(NAME)
	@nm $(NAME) | grep U

test: all
	g++ -std=c++11 main_test.cpp -I sources -o test && ./test $(SDL_NIBBLER_LIB)

coffee:
	@clear
	@echo ""
	@echo "                   ("
	@echo "	                     )     ("
	@echo "               ___...(-------)-....___"
	@echo '           .-""       )    (          ""-.'
	@echo "      .-''''|-._             )         _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "                 ("
	@echo "	                  )      ("
	@echo "               ___..(.------)--....___"
	@echo '           .-""       )   (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "               ("
	@echo "	                  )     ("
	@echo "               ___..(.------)--....___"
	@echo '           .-""      )    (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "             (         ) "
	@echo "	              )        ("
	@echo "               ___)...----)----....___"
	@echo '           .-""      )    (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'

.PHONY: all clean fclean re norme codesize test
