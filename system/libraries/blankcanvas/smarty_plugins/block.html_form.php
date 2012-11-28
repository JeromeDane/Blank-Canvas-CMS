<?php 

function smarty_block_html_form($params, $content, $template, &$repeat) {
	
	if($content) {
		
		$rawName = isset($params['name']) ? trim(htmlentities($params['name'])) : '';
		$name = $rawName ? 'name="'.$rawName.'" ' : '';
		$id = isset($params['id']) ? 'id="'.trim(htmlentities($params['id'])).'" ' : $rawName ? 'id="'.$rawName.'" ' : ''; 
		
		$attributes = SmartyUtils::getAttributes(array(
			'action', 
			'method' => 'post',
			'style',
			'enctype'
		), $params);
		
		return '<form '.$id.$name.$attributes.'>'.$content.'</form>';
	}
	
}

?>