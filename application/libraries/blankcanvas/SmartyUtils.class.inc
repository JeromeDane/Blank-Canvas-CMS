<?php 

class SmartyUtils {
	
	public static function getAttrFromParam($name, $params, $default = "") {
		if((isset($params[$name]) && trim($params[$name])) || $default != "") {
			$attribute = $name.'="';
			$attribute .= (isset($params[$name]) && trim($params[$name])) ? trim(htmlentities($params[$name])) : trim(htmlentities($default));
			$attribute .= '" ';
			return $attribute;	
		} else {
			return "";
		}
	}
	
	public static function getAttributes($attributes, $params) {
		if(is_string($attributes)) {
			$attributes = preg_split('/,/', $attributes);
		} else if(!is_array($attributes)) {
			throw new Exception('$attributes must be a comma delimited string or an array');
		}
		$html = " ";
		foreach($attributes as $k => $v) {
			
			$attribute = is_string($k) ? $k : $v;
			$default = is_string($k) ? $v : "";
			
			$html .= self::getAttrFromParam($attribute, $params, $default);
		}
		return $html;
	}
	
	public static function getIdAndNameAttributes($params) {
		$html = " ";
		$rawName = isset($params['name']) ? trim(htmlentities($params['name'])) : '';
		if($rawName) {
			$html .= 'name="'.$rawName.'" ';
		}
		$id = isset($params['id']) ? 'id="'.trim(htmlentities($params['id'])).'" ' : $rawName ? 'id="'.$rawName.'" ' : '';
		return $html.$id;
	}
	
}

?>