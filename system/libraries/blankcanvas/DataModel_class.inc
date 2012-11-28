<?php
/** {@link http://jeromedane.com Blank Canvas CMS}
 *
 * This file is part of the {@link http://jeromedane.com Blank Canvas CMS}.
 *
 * {@link http://jeromedane.com Blank Canvas CMS} is free software:
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
 * @author Jerome Dane <http://jeromedane.com>
 * @copyright Copyright (c) 2011, Jerome Dane
 * @license http://www.gnu.org/licenses/gpl.html GNU General Public License
 * @package BlankCanvas
 */

namespace BlankCanvas;

/**
 * Data model representing data entities (table structure and data records)
 *
 * @author 	Jerome Dane
 *
 */
class DataModel extends \ArrayObject {
	
	protected $_adapter;
	protected $_tableName;
	protected $_data = array();
	protected $_modifiedData = array();
	protected $_keyField;
	protected $_fieldPrefix;
	
	public function __construct(array $data = array(), $adapter = null, $tableName = null, $fieldPrefix = '', $keyField = 'id') {
		$this->_data = $data;
		$this->_adapter = $adapter;
		if(!isset($this->_tableName)) {
			$this->_tableName = $tableName ? $tableName : $this->_getType();
		}
		$this->_fieldPrefix = $fieldPrefix;
		$this->_keyField = $keyField;
	}
	
	/**
	 * Get the inner data as an array
	 */
	public function getData() {
		return $this->_data;
	}
	
	public function __get($key) {
		switch($key) {
			default:
				return isset($this->_data[$key]) ? $this->_data[$key] : null;
				
		}
	}
	
	public function __set($key, $val) {
		
		if(!isset($this->_data[$key]) || $this->_data[$key] != $val) {
			$this->_data[$key] = $val;
			$this->_modifiedData[$key] =& $this->_data[$key];
		}
	}
	
	public function delete() {
		$da = new DataAdapter();
		$da->execute("DELETE FROM ".$this->_tableName." WHERE ".$this->_keyField." = :".$this->_keyField, array(
			$this->_keyField => $this->$this->_keyField
		));
	}
	
	public function save() {
		// make sure there's something new to save
		if(count($this->_modifiedData) > 0) {
			// update existing content
			if(isset($this->_data[$this->_keyField])) {
				$this->_modifiedData[$this->_keyField] = $this->_data[$this->_keyField];
				DataAdapter::update($this->_tableName, $this->_modifiedData, "WHERE ".$this->_keyField." = :".$this->_keyField, array(
					$this->_keyField => $this->_data[$this->_keyField]
				));
			} else {
				// insert new records
				DataAdapter::insert($this->_tableName, $this->_data);
				$this->_data[$this->_keyField] = DataAdapter::getLastInsertId();
			}
		}
	}
	
	public function import($arg, $fields = false) {
		if(is_array($arg)) {
			foreach($arg as $k => $v) {
				if(!$fields || (is_array($fields) && in_array($k, $fields))) {
					$this->$k = $v;
				}
			}
		}
		
	}
	public function offsetGet($key) {
		return $this->__get($key);
	}
	
	public static function select($sql = "*", $params = array()) {
		$q = new DataSelector(new DataAdapter(), get_called_class());
		
		return $q->select($sql, $params)->from(self::_getType());
	}
	
	protected static function _getType() {
		$className = get_called_class();
		if(preg_match('/BlankCanvas\\\\([a-z]+)\\\\([a-z]+)/i', $className, $matches)) {
			return strtolower($matches[1]).'_'.strtolower($matches[2]);
		} else if(preg_match('/BlankCanvas\\\\([a-z]+)$/i', $className, $matches)) {
			return 'system_'.strtolower($matches[1]);
		}
	}
	
	public static function find($condition = '', $params = array()) {
		return self::select()->where($condition, $params);
	}
	
	public static function findSingle($condition, $params = array()) {
		return self::find($condition, $params)->fetch();
	}
	
}

?>