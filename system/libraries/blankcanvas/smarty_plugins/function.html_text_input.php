<?php 

function smarty_function_html_text_input($params, $template) {
	
	$class = isset($params['class']) ? ' '.trim(htmlentities($params['class'])) : ''; 

	$idAndName = SmartyUtils::getIdAndNameAttributes($params);
	
	$attributes = SmartyUtils::getAttributes('style', $params);
	
	$name = (isset($params['name']) && trim($params['name'])) ? trim($params['name']) : '';
	if(isset($_POST) && count($_POST) > 0 && $name && isset($_POST[$name])) {
		$value = ' value="'.trim(htmlentities($_POST[$name])).'" ';
	} else {
		$value = SmartyUtils::getAttrFromParam('value', $params);		
	}
	$val = (isset($params[$name]) && trim($params[$name])) ? trim($name) : '';
	
	return '<input '.$idAndName.' class="bcTextInput'.$class.'" type="text" '.$value.$attributes.'/>';
	
}

?>