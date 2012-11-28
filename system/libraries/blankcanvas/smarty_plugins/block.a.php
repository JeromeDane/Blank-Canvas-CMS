<?php

function smarty_block_a($params, $content, $template, &$repeat) {
	
	if($content) {
		
		if(isset($params['href'])) {
			$params['href'] = BlankCanvas\App::getRootUri() . preg_replace('/^\//', '', $params['href']);
		}
		
		$html = '<a';
		foreach($params as $name => $val) {
			$html .= ' '.$name.'="'.$val.'"';
		}
		$html .= ">".$content.'</a>';
		return $html;
	}
}

?>