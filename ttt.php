<?php
include("ttt_config.inc");
include("ttt_game.inc");
include("ttt_message.inc");

header('Content-Type: application/json');
$command = $_POST['command'];
	
//instanatiate objects
$config = new TTT_Config();
$outputMessage = new TTT_Message();
$gameObject = new TTT_Game($config->getDataFile());
	
if($command == '/ttt' and $_POST['token'] == $config->getToken()) {

    $input = $_POST['text'];
	$inputArray = explode(" ", $input);
	$sanitizedArray = array_values(array_filter($inputArray)); 
	
	if($sanitizedArray[0] == "play") {
		//first check if a game is going on
		if($gameObject->checkOngoing()) {
			echo $outputMessage->messageMaker("There is already a game going on");
			exit;
		}
		else {
			//check username validity, very simple way
			$data = array();
			if(strpos($sanitizedArray[1], "@") == 0) {
			
				$data['onGoing'] = 1;
				$data['player1'] = $_POST['user_name'];
				$data['player2'] = substr($sanitizedArray[1], 1); //need to check if user exists??
				$data['turn'] = $data['player1'];
				$data['moveCount'] = 0;
				//display empty board
				$data['existingMoves'] = array("A1" => ' ', "A2" => ' ', "A3" => ' ', "B1" => ' ', "B2" => ' ', "B3" => ' ', "C1" => ' ', "C2" => ' ', "C3" => ' ');
				
				$gameObject->setGameData($data);
				$gameObject->writeToINI($config->getDataFile());
				
				echo $gameObject->displayBoard("It is ".$_POST['user_name']."'s move");

			}
			else {
				echo $outputMessage->messageMaker("Please specify an opponent using @ username");
			}
			
		}
		
	}
	else if($sanitizedArray[0] == "move") {
	
		if(!$gameObject->checkOngoing()) {	
			echo $outputMessage->messageMaker("Please start a new game by playing someone in the channel");
			exit;
		}
		//check if the player has the right to move
		if($gameObject->checkTurn($_POST['user_name'])) {
			$move = $sanitizedArray[1];
			//now check validity of move, must be A,B,C follow by 1,2,3 and the space cannot be taken
			if(preg_match('/^[ABC][123]$/' , $move)) {
				
				//now check if the space is taken
				$existingMoves = $gameObject->getExistingMoves();
				if($existingMoves[$move] == ' ') {
					
					if($gameObject->checkTurn($gameObject->getPlayer1())) {
						$existingMoves[$move] = 'X';
						$gameObject->setTurn($gameObject->getPlayer2());
					}
					else {
						$existingMoves[$move] = 'O';
						$gameObject->setTurn($gameObject->getPlayer1());
					}
					$gameObject->addMoveCount();
					$gameObject->setExistingMoves($existingMoves);
					$gameObject->writeToINI($config->getDataFile());	
					
					//if the game is over..
					$winner = $gameObject->checkWin();
					if($winner != "NONE" or $gameObject->getMoveCount() == 9) {
						if($winner == 'player1') {
							$winmessage = "The winner is ".$gameObject->getPlayer1();
						}
						else if($winner == 'player2'){
							$winmessage = "The winner is ".$gameObject->getPlayer2();
						}
						else {
							$winmessage = "There are no winners";
						}
						
						echo $gameObject->displayBoard("The game is over, ".$winmessage);
						
						$gameObject->clearData();
						$gameObject->writeToINI($config->getDataFile());
	
						exit;
					
					}
					else {
						echo $gameObject->displayBoard("It is ".$gameObject->getTurn()."'s move");
					}
				}
				
			}
			
			
			echo $outputMessage->messageMaker("Sorry that is not a valid move");
	
		}
		else {
			echo $outputMessage->messageMaker("Sorry, it is not your turn. Please ask @".$gameObject->getTurn()." to make a move");
		}
	}
	else if($sanitizedArray[0] == "status") {
		
		if($gameObject->checkOngoing()) {
			echo $gameObject->displayBoard("It is ".$gameObject->getTurn()."'s move");
		}
		else {
			echo $outputMessage->messageMaker("There is no game in the channel");
			exit;
		}
	}
	else if($sanitizedArray[0] == "help") {
		echo $outputMessage->helpMessage();
		exit;
	}
	else {
		//command not recognized
		echo $outputMessage->messageMaker("Sorry, command not recognized. Type /ttt help for more information");
		exit;
	}
}



