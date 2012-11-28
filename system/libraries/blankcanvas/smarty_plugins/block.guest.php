<?php

use BlankCanvas\AppUser;
function smarty_block_guest($params, $content, $template, &$repeat) {
	
	if($content) {
		return AppUser::isLoggedIn() ? '' : $content;
	}
	
}

?>