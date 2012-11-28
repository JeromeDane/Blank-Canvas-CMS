<?php

use BlankCanvas\AppUser;
function smarty_block_registered($params, $content, &$template, &$repeat) {
	
	if($content && AppUser::isLoggedIn()) {
		if(isset($params['can'])) {
			return AppUser::can($params['can']) ? $content : '';
		}
		return $content;
	} else {
		return '';
	}
	
}

?>