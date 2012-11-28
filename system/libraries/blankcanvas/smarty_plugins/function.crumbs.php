<?php

function smarty_function_crumbs($params, $template) {
	
	$crumbs = BlankCanvas\View::getCrumbs();
	$html = "";
	$i = 0;
	foreach($crumbs as $k => $v) {
		$i++;
		$html .= $i < count($crumbs) ? '<a href="'.$v.'">'.$k.'</a> &gt; ' : (is_int($k) ? $v : $k);
	}
	
	return $html;
	return BlankCanvas\App::getRootUri() . $params['id'] .'/_' . (preg_replace('/[^\w]/', '_', $params['name']));
	
}

?>