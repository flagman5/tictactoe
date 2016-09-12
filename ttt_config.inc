<?php

class TTT_Config {

	public $token = 'mPTy9ZrGrItGjJKlzRQZvSFy';
    public $datafile = '/tmp/ttt.txt';
	
	public function getToken() {
		return $this->token;
	}
	
	public function getDataFile() {
		return $this->datafile;
	}
}
