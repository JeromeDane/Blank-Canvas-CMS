<?php
/** {@link http://blankcanvas.me/cms/ Blank Canvas CMS}
 *
 * This file is part of the {@link http://blankcanvas.me/cms/ Blank Canvas CMS}.
 *
 * {@link http://blankcanvas.me/cms/ Blank Canvas CMS} is free software:
 * you can redistribute it and/or modify it under the terms of the
 * {@link http://www.gnu.org/licenses/gpl.html GNU General Public License}
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Jerome Dane <http://blankcanvas.me/contact/>
 * @copyright Copyright (c) 2011, Jerome Dane
 * @license http://www.gnu.org/licenses/gpl.html GNU General Public License
 * @package BlankCanvas
 */

namespace BlankCanvas;


/**
 * EntityModel
 *
 * Wraps redbeans
 *
 * @author 	Jerome Dane
 *
 */
class Model extends \ActiveRecord\Model {
		
	public function __construct(array $attributes=array(), $guard_attributes=true, $instantiating_via_find=false, $new_record=true) {
		preg_match('/[^\\\\]+$/', get_class($this), $matches);
		$className = $matches[0];
		
		// @todo detect plugin context and generate context name
		$context = "system";
		
		// @todo add table prefixing
		$tableName = $context.'_'.(isset($this->plural) ? $this->plural : $className.'s');
		Model::$table_name = strtolower($tableName);

		if(!in_array('where', self::$VALID_OPTIONS))
			self::$VALID_OPTIONS[] = "where";
		
		parent::__construct($attributes, $guard_attributes, $instantiating_via_find, $new_record);
		
	}
	
	/**
	 * Import from an array
	 * 
	 * Allows for rapid importing from an array such as $_POST
	 * while limiting which fields to import through an 
	 * 
	 * @param unknown_type $importArray
	 * @param unknown_type $fields
	 */
	public function import($importArray, $fieldNames = null) {
		if(!is_array($importArray)) {
			throw new Exception('BlankCanvas\\Model must take an array as its first argument');
		}
		if(is_string($fieldNames)) {
			$fieldNames = preg_split('/,/', $fieldNames);
		}
		foreach($importArray as $k => $v) {
			if(!is_array($fieldNames) || (is_array($fieldNames) && in_array($k, $fieldNames))) {
				$this->$k = $v;
			}
		}
	}
	
	public static function find_by_pk($values, $options) {
		try {
			return parent::find_by_pk($values, $options);
		} catch(\ActiveRecord\RecordNotFound $e) {
			return null;
		}
	}

	
	
	
	
	
}

?>