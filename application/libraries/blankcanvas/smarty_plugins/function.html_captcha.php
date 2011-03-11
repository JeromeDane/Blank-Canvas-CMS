<?php 

function smarty_function_html_captcha($params, $template) {
	
	return \BlankCanvas\Captcha::getHtml();
	
}

?>