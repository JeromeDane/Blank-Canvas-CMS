<?php 

function smarty_function_html_password_input($params, $template) {
	
	$class = isset($params['class']) ? ' '.trim(htmlentities($params['class'])) : ''; 

	$idAndName = SmartyUtils::getIdAndNameAttributes($params);
	
	$attributes = SmartyUtils::getAttributes('style', $params);
	
	return '<input '.$idAndName.' class="bcTextInput'.$class.'" type="password" '.$attributes.'/>';
	
}

?>