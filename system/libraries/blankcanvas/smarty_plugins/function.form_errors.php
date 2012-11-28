<?php

use BlankCanvas\FormValidator;
function smarty_function_form_errors($params, $template) {
	
	$html = '';
	$errors = FormValidator::getErrors();
	if(count($errors) > 0) {
		$html .= '<ul class="form_errors">';
		foreach($errors as $error) {
			$html .= '<li class="error">'.htmlentities($error).'</li>';
		}
		$html .= '</ul>';
	}
	
	return $html;
	
}

?>