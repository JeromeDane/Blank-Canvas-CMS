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

require_once 'libraries/phpdatamapper/Base.php';

/**
 * EntityModel
 *
 * Wraps redbeans
 *
 * @author 	Jerome Dane
 *
 */
class Model extends \phpDataMapper_Entity {
	
	protected static $_globalAdapter;
	protected static $_innerModel;
	protected static $_mapper;
	protected static $_className;
	protected static $_plural;
	protected static $_tableName;
	protected static $_fields;
	
	public static function connect($server, $database, $user, $password) {
		require_once 'libraries/phpdatamapper/Adapter/Mysql.php';
		self::$_globalAdapter = new \phpDataMapper_Adapter_Mysql($server, $database, $user, $password);
	}
	
	public static function create() {
		return self::_getMapper()->get();
	}
	
	public static function find($input = null) {
		if (is_int($input)) {
			return self::_getMapper()->get($input);
		}
		return self::_getMapper()->select();
	}
	
	public static function updateDatabase() {
		self::_getMapper()->migrate();
	}
	
	protected static function &_getMapper() {
		if (is_null(self::$_mapper)) {
			//$className = get_called_class();
			
			$fullClassName = get_called_class();
			
			preg_match('/[^\\\\]+$/', $fullClassName, $matches);
			$className = $matches[0];
			
			// @todo detect plugin context and generate context name
			$context = "system";
			
			// @todo add table prefixing
			$tableName = $context . '_' . (isset(self::$_plural) ? self::$_plural : $className . 's');
			$tableName = strtolower($tableName);
			
			$inst = new $fullClassName();
			$preparedFields = self::_prepareFields($inst->getFields());
			$inst = null;
			
			self::$_mapper = new Mapper(self::$_globalAdapter, null, $fullClassName, $tableName, $preparedFields);
			
		}
		return self::$_mapper;
	}
	
	protected static function _prepareFields($fields) {
		
		// impose ID as unique promary key
		$fields['id'] = array(
			'type'     => 'int',
			'primary'  => true,
			'serial'   => true,
			'unsigned' => true
		);
		
		// Default settings for all fields
		$fieldDefaults = array(
			'type'     => 'string',
			'default'  => null,
			'length'   => null,
			'required' => false,
			'null'     => true,
			'unsigned' => false,
			'primary'  => false,
			'index'    => false,
			'unique'   => false,
			'serial'   => false,
			'relation' => false
		);
		
		// Type default overrides for specific field types
		$fieldTypeDefaults = array(
			'string' => array(
				'length' => 255
			),
			'float'  => array(
				'length' => array(
					10,
					2
				)
			),
			'int'    => array(
				'length'   => 10,
				'unsigned' => true
			)
		);
		
		$returnFields = array();
		foreach ($fields as $fieldName => $fieldOpts) {
			// Format field will full set of default options
			if (isset($fieldInfo['type']) && isset($fieldTypeDefaults[$fieldOpts['type']])) {
				// Include type defaults
				$fieldOpts = array_merge($fieldDefaults, $fieldTypeDefaults[$fieldOpts['type']], $fieldOpts);
			} else {
				// Merge with defaults
				$fieldOpts = array_merge($fieldDefaults, $fieldOpts);
			}
			
			// Store relations (and remove them from the mix of regular fields)
			if ($fieldOpts['type'] == 'relation') {
				$this->_relations[$fieldName] = $fieldOpts;
				continue; // skip, not a field
			}
			
			$returnFields[$fieldName] = $fieldOpts;
		}
		return $returnFields;
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
		if (!is_array($importArray)) {
			throw new Exception('BlankCanvas\\Model must take an array as its first argument');
		}
		if (is_string($fieldNames)) {
			$fieldNames = preg_split('/,/', $fieldNames);
		}
		foreach ($importArray as $k => $v) {
			if (!is_array($fieldNames) || (is_array($fieldNames) && in_array($k, $fieldNames))) {
				$this->$k = $v;
			}
		}
	}
	
	public function save() {
		self::_getMapper()->save($this);
	}
	
	public function __get($property) {
		if ($property == 'id') {
			return (int) parent::__get($property);
		}
		return parent::__get($property);
	}
	
}

class Mapper extends \phpDataMapper_Base {
	public function __construct($adapter = null, $adapterRead = null, $entityClassName = null, $tableName = null, $fields) {
		
		$this->_primaryKey = 'id';
		
		$this->_datasource = $tableName;
		$this->_entityClass = $entityClassName;
		$this->_fields = $fields;
		
		$adapter = is_null($adapter) ? self::$_globalAdapter : $adapter;
		parent::__construct($adapter, $adapterRead);
	}
	
}

?>