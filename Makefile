CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -g

play: Play.o Game.o ChessGame.o KOTHChessGame.o SpookyChessGame.o ChessPiece.o
	$(CXX) Play.o Game.o ChessGame.o KOTHChessGame.o SpookyChessGame.o ChessPiece.o -g -o play

Play.o: Play.cpp Game.h ChessGame.h Prompts.h
	$(CXX) $(CXXFLAGS) -c Play.cpp

Game.o: Game.cpp Game.h Piece.h Prompts.h Enumerations.h Terminal.h
	$(CXX) $(CXXFLAGS) -c Game.cpp

ChessGame.o: ChessGame.cpp Game.h ChessGame.h Piece.h ChessPiece.h Prompts.h Enumerations.h
	$(CXX) $(CXXFLAGS) -c ChessGame.cpp

KOTHChessGame.o: KOTHChessGame.cpp Game.h KOTHChessGame.h Piece.h ChessPiece.h Prompts.h Enumerations.h
	$(CXX) $(CXXFLAGS) -c KOTHChessGame.cpp

SpookyChessGame.o: SpookyChessGame.cpp Game.h SpookyChessGame.h Piece.h ChessPiece.h Prompts.h Enumerations.h
	$(CXX) $(CXXFLAGS) -c SpookyChessGame.cpp

ChessPiece.o: ChessPiece.cpp ChessPiece.h Enumerations.h Piece.h
	$(CXX) $(CXXFLAGS) -c ChessPiece.cpp

clean:
	rm *.o play

