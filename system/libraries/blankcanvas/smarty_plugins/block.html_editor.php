<?php

function smarty_block_html_editor($params, $content, $template, &$repeat) {
	
	if($content) {
		$class = isset($params['class']) ? ' '.trim(htmlentities($params['class'])) : '';
	
		$idAndName = SmartyUtils::getIdAndNameAttributes($params);
		
		$attributes = SmartyUtils::getAttributes('style', $params);
		
		$name = (isset($params['name']) && trim($params['name'])) ? trim($params['name']) : '';
		if(isset($_POST) && count($_POST) > 0 && $name && isset($_POST[$name])) {
			$content = trim(htmlentities($_POST[$name]));
		}
		$val = (isset($params[$name]) && trim($params[$name])) ? trim($name) : '';
		
		return '<textarea '.$idAndName.' class="bcTextArea'.$class.'" '.$attributes.'>'.$content.'</textarea>';
	}
}

?>