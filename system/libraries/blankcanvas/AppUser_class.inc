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
	
	public static function startSession() {
		session_start();
		self::$_user = isset($_SESSION['system_app_user']) ? $_SESSION['system_app_user'] : false;
	}
	
	public static function can($context, $permission = false) {
		if($permission) {
			return self::$_user->id == 1;
		} else if(preg_match('/^([^_]+)_(.+)$/', $context, $matches)) {
			return self::can($matches[1], $matches[0]);
		}
		return false;
	}
	
	public static function isLoggedIn() {
		return (isset($_SESSION['system_app_user']) && $_SESSION['system_app_user']);
	}
	
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
		
		$hashedPassword = md5($password);
		
		$user = User::findSingle("email = :email AND password = :password", array(
			"email" => $email,
			"password" => $hashedPassword
		));
		if($user) {
			self::load($user);
			return true;
		} else {
			if(!isset($_SESSION['system_app_user_failed_logins'])) {
				$_SESSION['system_app_user_failed_logins'] = 0;
			}
			$_SESSION['system_app_user_failed_logins']++;
		}
	}
	public static function load($user) {
		$user->last_ip = $_SERVER['REMOTE_ADDR'];
		$user->save();
		self::$_user = $user;
		$_SESSION['system_app_user'] = self::$_user;
		$_SESSION['system_app_user_failed_logins'] = 0;
	}
	public static function get($property) {
		if(self::isLoggedIn()) {
			return self::$_user->$property;
		}
	}
	public static function set($property, $value) {
		if(self::isLoggedIn()) {
			self::$_user->$property = $value;
		}
	}
	public static function save() {
		if(self::isLoggedIn()) {
			self::$_user->save();
			//$_SESSION['system_app_user'] = self::$_user;
		}
	}
	public static function getId() {
		return self::isLoggedIn() ? (int) self::get('id') : 0;
	}
	public function &getInstance() {
		return self::$_user;
	}
	public function getNumFailedLoginAttempts() {
		return isset($_SESSION['system_app_user_failed_logins']) ? $_SESSION['system_app_user_failed_logins'] : 0;
	}
	public function unload() {
		self::$_user = false;
		unset($_SESSION['system_app_user']);
	}
	
}

?>