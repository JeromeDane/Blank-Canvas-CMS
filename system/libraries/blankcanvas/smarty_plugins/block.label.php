<?php

use BlankCanvas\FormValidator;
function smarty_block_label($params, $content, $template, &$repeat) {
	
	if($content) {
		
		$for = isset($params['for']) ? 'for="'.$params['for'].'" ' : '';
		$class = '';
		$title = '';
		if(isset($params['for'])) {
			$errors = FormValidator::getErrors($params['for']);
			if(count($errors)) {
				$class .= 'error ';
			}
		}
		
		return '<label '.$for.$class.' class="'.$class.'">'.$content.'</label>';
	}
	
}

?>