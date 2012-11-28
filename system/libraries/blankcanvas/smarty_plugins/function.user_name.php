<?php

function smarty_function_user_name($params, $template) {
	
	return htmlentities(\BlankCanvas\AppUser::get('name'));
}

?>