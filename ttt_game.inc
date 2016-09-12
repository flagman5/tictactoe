<?php

class TTT_Game {

	public $gameData;
	
	public function __construct($dataFile) {
		$this->gameData = parse_ini_file($dataFile);
	}
	
	public function checkOngoing() {
		if($this->gameData['onGoing'] == 1) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	
	public function checkTurn($username) {
		if($this->gameData['turn'] == $username) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	
	public function getTurn() {
		return $this->gameData['turn'];
	}	
	
	public function getExistingMoves() {
		return $this->gameData['existingMoves'];
	}
	
	public function getPlayer1() {
		return $this->gameData['player1'];
	}
	
	public function getPlayer2() {
		return $this->gameData['player2'];
	}
	
	public function getMoveCount() {
		return $this->gameData['moveCount'];
	}
	
	public function setTurn($player) {
		$this->gameData['turn'] = $player;
		return;
	}
	
	public function setExistingMoves($array) {
		$this->gameData['existingMoves'] = $array;
		return;
	}
	
	public function addMoveCount() {
		$this->gameData['moveCount']++;
		return;
	}
	
	public function checkWin() {
		// first check the horizontals
		$slice = array_unique(array_slice($this->gameData['existingMoves'], 0, 3));
		if(count($slice) == 1) {
			return $this->getWinner($slice);
		}
		
		$slice = array_unique(array_slice($this->gameData['existingMoves'], 3, 3));
		if(count($slice) == 1) {
			return $this->getWinner($slice);
		}
		
		$slice = array_unique(array_slice($this->gameData['existingMoves'], 6, 3));
		if(count($slice) == 1) {
			return $this->getWinner($slice);
		}
		
		//now check the verticals
		$slice = array();
		array_push($slice, $this->gameData['existingMoves']['A1'], $this->gameData['existingMoves']['B1'], $this->gameData['existingMoves']['C1']);
		$slice = array_unique($slice);
		if(count($slice) == 1) {
			return $this->getWinner($slice);
		}
		$slice = array();
		array_push($slice, $this->gameData['existingMoves']['A2'], $this->gameData['existingMoves']['B2'], $this->gameData['existingMoves']['C2']);
		$slice = array_unique($slice);
		if(count($slice) == 1) {
			return $this->getWinner($slice);
		}
		$slice = array();
		array_push($slice, $this->gameData['existingMoves']['A3'], $this->gameData['existingMoves']['B3'], $this->gameData['existingMoves']['C3']);
		$slice = array_unique($slice);
		if(count($slice) == 1) {
			return $this->getWinner($slice);
		}
		
		//now check the 2 diags
		$slice = array();
		array_push($slice, $this->gameData['existingMoves']['A1'], $this->gameData['existingMoves']['B2'], $this->gameData['existingMoves']['C3']);
		$slice = array_unique($slice);
		if(count($slice) == 1) {
			return $this->getWinner($slice);
		}
		$slice = array();
		array_push($slice, $this->gameData['existingMoves']['A3'], $this->gameData['existingMoves']['B2'], $this->gameData['existingMoves']['C1']);
		$slice = array_unique($slice);
		if(count($slice) == 1) {
			return $this->getWinner($slice);
		}
		
		return "NONE";
	}
	
	public function getWinner($array) {
		if(reset($array) == 'X') {
			return "player1";
		}
		else if(reset($array) == 'O'){
			return "player2";
		}
		else {
			return "NONE";
		}
	}
	public function setGameData($data) {
		$this->gameData['onGoing'] = $data['onGoing'];
		$this->gameData['player1'] = $data['player1'];
		$this->gameData['player2'] = $data['player2'];
		$this->gameData['turn'] = $data['turn'];
		$this->gameData['moveCount'] = $data['moveCount'];
		$this->gameData['existingMoves'] = $data['existingMoves'];
		
		return;
	}
	
	public function writeToINI($dataFile) {
		//write back into file
		$this->write_php_ini($this->gameData, $dataFile);
		
		return;
	}
	
	public function displayBoard($message) {
		$array = array("response_type" => "in_channel",
					   "text" => "```| ".$this->gameData['existingMoves']['A1']." | ".$this->gameData['existingMoves']['A2']." | ".$this->gameData['existingMoves']['A3']." |\n|---+---+---|\n| ".
										$this->gameData['existingMoves']['B1']." | ".$this->gameData['existingMoves']['B2']." | ".$this->gameData['existingMoves']['B3']." |\n|---+---+---|\n| ".
										$this->gameData['existingMoves']['C1']." | ".$this->gameData['existingMoves']['C2']." | ".$this->gameData['existingMoves']['C3']." |```",
						"attachments" => array("0" => array("text" => $message))
					   );
		return json_encode($array);
	}

	public function write_php_ini($array, $file)
	{
		$res = array();
		foreach($array as $key => $val)
		{
			if(is_array($val))
			{
				//$res[] = "[$key]";
				foreach($val as $skey => $sval) $res[] = "$key"."[".$skey."]  = ".(is_numeric($sval) ? $sval : '"'.$sval.'"');
			}
			else $res[] = "$key = ".(is_numeric($val) ? $val : '"'.$val.'"');
		}
		$this->safefilerewrite($file, implode("\r\n", $res));
	}

	public function safefilerewrite($fileName, $dataToSave)
	{    if ($fp = fopen($fileName, 'w'))
		{
			$startTime = microtime(TRUE);
			do
			{  $canWrite = flock($fp, LOCK_EX);
			   // If lock not obtained sleep for 0 - 100 milliseconds, to avoid collision and CPU load
			   if(!$canWrite) usleep(round(rand(0, 100)*1000));
			} while ((!$canWrite)and((microtime(TRUE)-$startTime) < 5));

			//file was locked so now we can store information
			if ($canWrite)
			{            fwrite($fp, $dataToSave);
				flock($fp, LOCK_UN);
			}
			fclose($fp);
		}

	}
	
	public function clearData() {
		$this->gameData['onGoing'] = 0;
		$this->gameData['player1'] = '';
		$this->gameData['player2'] = '';
		$this->gameData['turn'] = '';
		$this->gameData['moveCount'] = 0;
		$this->gameData['existingMoves'] = array();
		
		return;
	}
}
