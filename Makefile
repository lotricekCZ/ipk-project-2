CXX = g++
CXXFLAGS = -fPIC -shared -std=c++20 -I./src
LDFLAGS = -L./src -lmessage

LIBDIR = lib
SRCDIR = src
OBJDIR = obj

MESSAGE_SRC = $(SRCDIR)/formats/message.cpp
ENCODER_SRC = $(SRCDIR)/encoders/TCP_encoder.cpp
DECODER_SRC = $(SRCDIR)/decoders/TCP_decoder.cpp
FSM_SRC = $(SRCDIR)/fsm/fsm.cpp

ENCODER_LIB = $(LIBDIR)/libencoder.so
DECODER_LIB = $(LIBDIR)/libdecoder.so
FSM_LIB = $(LIBDIR)/libfsm.so

.PHONY: all main clean $(MESSAGE_LIB) # $(ENCODER_LIB) $(DECODER_LIB) $(FSM_LIB)

all: $(MESSAGE_LIB) #$(ENCODER_LIB) $(DECODER_LIB) $(FSM_LIB)

main: $(MESSAGE_LIB) # $(ENCODER_LIB) $(DECODER_LIB) $(FSM_LIB)
	$(CXX) -I$(SRCDIR) -L$(LIBDIR) $< -lmessage -lencoder -ldecoder -lfsm -o $@


$(ENCODER_LIB): $(ENCODER_SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

$(DECODER_LIB): $(DECODER_SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

$(FSM_LIB): $(FSM_SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

$(MESSAGE_LIB): $(MESSAGE_SRC)
	$(CXX) $(CXXFLAGS) $< -o $@
	
clean:
	rm -f $(LIBDIR)/*.so