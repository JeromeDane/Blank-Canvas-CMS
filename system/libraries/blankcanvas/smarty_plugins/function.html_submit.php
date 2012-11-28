<?php 

function smarty_function_html_submit($params, $template) {
	
	$rawName = isset($params['name']) ? trim(htmlentities($params['name'])) : '';
	$name = $rawName ? 'name="'.$rawName.'" ' : '';
	$id = isset($params['id']) ? 'id="'.trim(htmlentities($params['id'])).'" ' : $rawName ? 'id="'.$rawName.'" ' : '';
	
	$class = isset($params['class']) ? ' '.trim(htmlentities($params['class'])) : ''; 

	$attributes = SmartyUtils::getAttributes(array(
		'value'	=> 'Submit',
		'style'
	), $params);
	
	return '<input class="bcSubmit'.$class.'" type="submit" '.$name.$id.$attributes.'/>';
	
}

?>