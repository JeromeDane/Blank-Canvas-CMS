<?php

namespace BlankCanvas;

// @todo document AppUser class
class AppUser {
	
	/**
	 * The actual user database object. Null unless loaded
	 * @var BlankCanvas\User
	 */
	protected static $_user;
	protected static $_sessionData;
	
	/**
	 * Login
	 * 
	 * Log a user in using their login and password
	 * 
	 * @param	string	$email		The user's email
	 * @param 	string	$password 	The user's password
	 * 
	 * @return	boolean		True if successful, false if not
	 */
	static function login($email, $password) {
		
		require_once 'application/models/user.model.inc';
		
		$hashedPassword = $password;
		
		self::$_user = User::find(array(
			'where' => array("email = ? AND password = ?", $email, $hashedPassword)
		)); 
		var_dump(self::$_user );
		
		//self::$_user = User::find_by_email($email); 
		if(self::$_user) {
			return true;
		} else {
			return false;
		}
	}
	
}

?>