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
class DataSelector implements \Iterator, \Countable {

	protected $_adapter;
	protected $_result;
	protected $_pieces;
	protected $_tableName;
	protected $_className;

	public function __construct($adapter = null, $className = false) {
		$this->_className = $className;
		$this->_adapter = is_null($adapter) ? new DataAdapter() : $adapter;
		$this->_pieces = array();
		$this->_pieces['innerJoins'] = array();
	}

	public function select($sql = "*", $parameters = array()) {
		$this->_setPiece('select', $sql, $parameters);
		return $this;
	}

	public function from($sql, $parameters = array()) {
		$this->_setPiece('from', $sql, $parameters);
		return $this;
	}
	
	public function join($tableName, $sql, $inner = true) {
		$this->_pieces['innerJoins'][] = " ".($inner ? "INNER" : "LEFT")." JOIN ".$tableName." ON ".$sql;
		return $this;
	}

	public function limit($int) {
		$this->_pieces['limit'] = (int) $int;
		return $this;
	}
	public function offset($int) {
		$this->_pieces['offset'] = (int) $int;
		return $this;
	}
	public function group($sql) {
		$this->_pieces['group'] = $sql;
		return $this;
	}
	public function where($sql, $parameters = array(), $reset = false) {
		if(!isset($this->_pieces['where']) || $reset) {
			$this->_pieces['where'] = array();
		}
		$this->_pieces['where'][] = array(
			'sql' => $sql,
			'parameters' => $parameters
		);
		return $this;
	}

	public function order($sql, $parameters = array()) {
		if(preg_match('/^\w+$/', $sql)) {
			$sql = '`'.$sql.'`';
		}
		$this->_setPiece('order', $sql, $parameters);
		return $this;
	}

	public function getQueryString() {
		$this->_checkForResult();
		return $this->_result->getQueryString();
		
	}

	public function getArray() {
		$records = array();
		foreach($this as $record) {
			$records[] = $record;
		}
		return $records;
	}
	
	public function fetch() {
		$this->_checkForResult();
		return $this->_result->fetch();
	}
	
	public function getNumRows() {
		$this->_checkForResult();
		return $this->_result->getNumRows();
	}

	public function count($rewind = false) {
		$this->_checkForResult($rewind);
		return $this->_result->count();
	}
	public function current() {
		$this->_checkForResult();
		return $this->_result->current();
	}
	public function key() {
		$this->_checkForResult();
		return $this->_result->key();
	}
	public function next() {
		$this->_checkForResult();
		return $this->_result->fetch();
	}
	public function rewind() {
		$this->_checkForResult(true);
		$this->_result->rewind();
		return $this;
	}
	public function valid() {
		$this->_checkForResult();
		return $this->_result->valid();
	}
	
	protected function _setPiece($name, $sql, $parameters = array()) {
		$this->_pieces[$name] = array(
			'sql' => $sql,
			'parameters' => is_array($parameters) ? $parameters : array($parameters)
		);
	}

	protected function _checkForResult($reload = false) {
		if($reload || is_null($this->_result)) {
			$parameters = array();
	
			if(!isset($this->_pieces['select'])) {
				throw new \Exception("Missing select statement");
			}
			$query = "SELECT " . $this->_pieces['select']['sql'];
			$parameters = array_merge($parameters, $this->_pieces['select']['parameters']);
	
			if(!isset($this->_pieces['from'])) {
				throw new \Exception("Missing from statement");
			}
			$query .= " FROM ".$this->_pieces['from']['sql'];
			$parameters = array_merge($parameters, $this->_pieces['from']['parameters']);
	
			foreach($this->_pieces['innerJoins'] as $joinSql) {
				$query .= " ".$joinSql;
			}
			
			if(isset($this->_pieces['where'])) {
				$i = 0;
				foreach($this->_pieces['where'] as $where) {
					$i++;
					if(trim($where['sql'])) {
						$query .= ($i == 1 ? " WHERE " : "") . " ".$where['sql'].' ';
						if($i < count($this->_pieces['where'])) {
							$query .= " AND ";
						}
						$parameters = array_merge($parameters, $where['parameters']);
					}
				}
			}
			if(isset($this->_pieces['group'])) {
				$query .= " GROUP BY ".$this->_pieces['group'];
			}
			if(isset($this->_pieces['order'])) {
				$query .= " ORDER BY ".$this->_pieces['order']['sql'];
				$parameters = array_merge($parameters, $this->_pieces['order']['parameters']);
			}
			if(isset($this->_pieces['limit'])) {
				$query .= " LIMIT ".$this->_pieces['limit'];
				if(isset($this->_pieces['offset'])) {
					$query .= " OFFSET ".$this->_pieces['offset'];
				}
				
			}
			
			// @todo add parameters
			$this->_result = $this->_adapter->executeSelect($query, $parameters, $this->_className);
		}
	}

}

?>