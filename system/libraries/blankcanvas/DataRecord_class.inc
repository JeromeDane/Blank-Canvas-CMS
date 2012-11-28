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
 * DataAdapter
 *
 * @author 	Jerome Dane
 *
 */
class DataRecord {
	
	protected $_adapter;
	protected $_tableName;
	protected $_data;
	protected $_keyField;
	protected $_fieldPrefix;
	
	public function __construct(array $data, $adapter = null, $tableName = null, $fieldPrefix = '', $keyField = 'id') {
		$this->_data = $data;
		$this->_adapter = $adapter;
		$this->_tableName = $tableName;
		$this->_fieldPrefix = $fieldPrefix;
		$this->_keyField = $keyField;
	}
	
	public function getData() {
		return $this->_data;
	}
	
	public function __get($key) {
		switch($key) {
			default:
				return isset($this->_data[$key]) ? $this->_data[$key] : null;
				
		}
	}
		
}

?>