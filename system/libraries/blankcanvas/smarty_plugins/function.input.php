<?php

use BlankCanvas\AppUser;
use BlankCanvas\FormValidator;
function smarty_function_input($params, $template) {
	
	$class = isset($params['class']) ? ' '.trim(htmlentities($params['class'])) : '';

	$idAndName = SmartyUtils::getIdAndNameAttributes($params);
	
	$attributes = SmartyUtils::getAttributes('style', $params);
	
	$name = (isset($params['name']) && trim($params['name'])) ? trim($params['name']) : '';
	if(isset($_POST) && count($_POST) > 0 && $name && isset($_POST[$name])) {
		if(!isset($params['type']) || $params['type'] != 'password') {
			$value = ' value="'.trim(htmlentities($_POST[$name])).'" ';
		} else {
			$value = '';
		}
	} else {
		$value = SmartyUtils::getAttrFromParam('value', $params);
	}
	$val = (isset($params[$name]) && trim($params[$name])) ? trim($name) : '';
	
	$errors = array();
	if($name) {
		// add validation conditions
		foreach(FormValidator::getTypes() as $condition) {
			if(isset($params[$condition]) && $params[$condition]) {
				FormValidator::addCondition($name, $condition, $params[$condition]);
			}
		}
		$errors = FormValidator::getErrors($name);
		if(count($errors) > 0) {
			$class .= ' error';
		}
	}
	
	$type = isset($params['type']) ? ' type="'.$params['type'].'" ' : ' type="text" ';
	$title = isset($params['title']) ? ' title="'.$params['title'].'" ' : '';
	
	return '<input '.$idAndName.' class="bcTextInput'.$class.'" '.$type.$title.$value.$attributes.'
		onkeyup="this.className = this.className.replace(/\\berror\\b/, \'\');"/>';
	
}

?>