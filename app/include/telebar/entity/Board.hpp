#ifdef BOARD_HPP
#define BOARD_HPP
 #include "piece.hpp"

#include <vector>


class Board{

	private:

		std::vector<Piece> _board;
		

	public: 

		Board();

		

    	bool putPiece(Piece &p);

};






}

































#endif