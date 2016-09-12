<?php

class TTT_Message {

	public function messageMaker($message) {
		$array = array("response_type" => "in_channel",
				   "text" => $message,
					"link_names" => 1
				);
		return json_encode($array);
	}
	public function helpMessage() {
		$array = array("response_type" => "ephemeral",
					   "text" => "How to use /ttt",
					   "attachments" => array("0" => array("text" => "To start the game, use `/ttt play @username`.\n To check the status of an on-going game, use `/ttt status`.\n During a game, only the player who is at his turn can make a move. To make a move, use `/ttt move` follow by the location of the move on the board. \nThe location of the move is specified by the top row A, middle row B, third row C, concatenated with column 1, column 2, column 3. \nFor example, the top left corner of the board would be 'A1', and the middle box would be 'B2'.\nA complete move command would be something like `/ttt move A1`."))
					   );
		return json_encode($array);			 
	}
}
