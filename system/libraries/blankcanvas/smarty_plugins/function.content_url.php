<?php

function smarty_function_content_url($params, $template) {
	
	if(!isset($params['name'])) {
		if($content = \BlankCanvas\Content::getById($params['id'])) {
			$params['name'] = $content->name;	
		}
	}
	
	return BlankCanvas\App::getRootUri() . $params['id'] .'/_' . (preg_replace('/[^\w]/', '_', $params['name']));
	
}

?>