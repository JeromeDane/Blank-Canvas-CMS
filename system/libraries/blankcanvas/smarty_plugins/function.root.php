<?php

use BlankCanvas\App;
function smarty_function_root($params, $template) {
	
	return App::getRootUri();
}

?>