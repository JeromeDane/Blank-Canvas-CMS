<?php

function smarty_function_page_numbers($params, $template) {
	
	$controller = \BlankCanvas\App::getController('system', 'page');
	return $controller->numbers($params);
	
}

?>