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
class DataResult implements \Iterator, \Countable {

	protected $_statement;
	protected $_executed = false;
	protected $_currentRecord;
	protected $_currentRecordFetched = false;
	protected $_className;
	
	protected $_iteratorIndex = 0;
	
	public function __construct($statement, $className = false) {
		$this->_className = $className;
		$this->_statement = $statement;
		
		$this->position = 0;
	}
	
	public function getNumRows($rewind = false) {
		$this->_checkForExecution($rewind);
		return $this->_statement->rowCount();
	}
	
	public function getQueryString() {
		return $this->_statement->queryString;
	}
	
	public function fetch() {
		$this->_checkForExecution();
		$data = $this->_statement->fetch(\PDO::FETCH_ASSOC);
		if($data) {
			$this->_currentRecord = $this->_className ? new $this->_className($data) : new DataModel($data);
		} else {
			$this->_currentRecord = false;
		}
		return $this->_currentRecord;
	}
	
	public function count($rewind = false) {
		return $this->getNumRows($rewind);
	}
	public function current() {
		return $this->_currentRecord;
	}
	public function key() {
		return $this->_iteratorIndex;
	}
	public function next() {
		$this->_iteratorIndex++;
		return $this->fetch();
	}
	public function rewind() {
		if($this->_iteratorIndex != 0) {
			$this->_iteratorIndex = 0;
			$this->_statement->execute();
		}
		$this->fetch();
	}
	public function setStatement($statement) {
		$this->_statement = $statement;
	}
	public function valid() {
		return (bool) $this->current();
	}
	private function _checkForExecution($reload = false) {
		if($reload || !$this->_executed) {
			$this->_statement->execute();
			$this->_executed = true;
		}
	}
	
}

?>