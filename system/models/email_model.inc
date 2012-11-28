<?php

namespace BlankCanvas;

class Email extends DataModel {
	
	public static function validateAddress($email) {
		return preg_match('/^([_a-z0-9-\+\.]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/i', $email);
	}
}

?>