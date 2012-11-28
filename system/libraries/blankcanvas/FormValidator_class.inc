<?php

namespace BlankCanvas;

class FormValidator {
	
	protected static $_customErrors = array();
	protected static $_conditions;
	protected static $_conditionTypes = array(
		'required',
		'match',
		'email',
		'minlength',
		'maxlength',
		'custom'
	);
	
	public static function addError($target, $message) {
		if(!isset(self::$_customErrors[$target])) {
			self::$_customErrors[$target] = array();
		}
		self::$_customErrors[$target][] = $message;
		self::addCondition($target, 'custom', $message);
	}
	public static function addCondition($target, $type, $message) {
		
		self::_loadConditions();
		if(!isset(self::$_conditions[$target]))
			self::$_conditions[$target] = array();
		self::$_conditions[$target][$type] = $message;
		self::_saveConditions();
	}
	
	
	
	public static function check($target, $condition = false) {
		if(!$condition) {
			$passed = true;
			foreach(self::getTypes() as $condition) {
				$passed = self::check($target, $condition) ? $passed : false;
			}
			return $passed;
		} else {
			self::_loadConditions();
			if($condition == 'custom' && isset(self::$_customErrors[$target])) {
				return false;
			}
			$relevant = (isset(self::$_conditions[$target]) && isset(self::$_conditions[$target][$condition]) && self::$_conditions[$target][$condition]);
			if($relevant && isset($_POST[$target])) {
				$c = self::$_conditions[$target];
				$test = trim($_POST[$target]);
				switch($condition) {
					case 'custom':
					case 'required':
						return $test != '';
					case 'minlength':
						if(preg_match('/\d+/', $c['minlength'], $matches)) {
							return strlen($test) >= (int) $matches[0];
						}
						return true;
					case 'maxlength':
						if(preg_match('/\d+/', $c['maxlength'], $matches)) {
							return strlen($test) <= (int) $matches[0];
						}
						return true;
					case 'match':
						if(!preg_match('/_match$/', $target)) return true;
						$orig = preg_replace('/_match$/', '', $target);
						return (!isset($_POST[$orig]) ||
								(!self::check($orig, 'required') && trim($_POST[$orig]) == '') ||
								trim($_POST[$orig]) == $test
						);
					case 'email':
						if($test == '') {
							return true;
						} else {
							return Email::validateAddress(trim($_POST[$target]));
						}
				}
			}
			return true;
		}
	}
	public static function passed() {
		$passed = true;
		foreach($_POST as $k => $v) {
			$passed = self::check($k) ? $passed : false;
		}
		return $passed;
	}
	
	public static function getTypes() {
		return self::$_conditionTypes;
	}
	public static function getErrors($target = false) {
		if($target) {
			return self::_getTargetErrors($target);
		}
		self::_loadConditions();
		$errors = array();
		if(isset($_POST)) {
			foreach($_POST as $k => $v) {
				$errors = array_merge($errors, self::getErrors($k));
			}
		}
		return $errors;
	}
	protected function _saveConditions() {
		$_SESSION['system_form_validator_conditions'] = self::$_conditions;
	}
	protected function _loadConditions() {
		if(isset($_SESSION['system_form_validator_conditions'])) {
			self::$_conditions = $_SESSION['system_form_validator_conditions'];
		} else {
			self::$_conditions = array();
		}
	}
	protected static function _getTargetErrors($target) {
		self::_loadConditions();
		$errors = array();
		foreach (self::getTypes() as $condition) {
			if(!self::check($target, $condition))
				$errors[] = self::$_conditions[$target][$condition];
		}
		return $errors;
	}
	
}

?>