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
 * Data adapter for access to stored information (database)
 *
 * @author 	Jerome Dane
 *
 */
class DataAdapter {

	protected static $_defaultConnection;
	
	public static function connect($dsn, $username = "root", $password = "", $options = array()) {
		
		self::$_defaultConnection = new \PDO($dsn, $username, $password, $options);
		
		self::$_defaultConnection->setAttribute(\PDO::ATTR_ERRMODE, \PDO::ERRMODE_EXCEPTION);
		
	}
	
	protected $_connection;
	
	/**
	 * Execute SQL select statement with optional parameters and return the result
	 *
	 * @param string $sql The SQL to be executed
	 * @param array $parameters Optional array of PDO style parameters
	 * @return DataResult
	 */
	public function executeSelect($sql, array $parameters = array(), $className = false) {
		
		$statement = $this->_connection->prepare($sql);
		
		$paramNumber = 0;
		foreach($parameters as $k => $v) {
			
			if(is_int($k)) $paramNumber++;
			$param = is_int($k) ? $paramNumber : ':' . $k;
			
			$statement->bindParam($param, $parameters[$k]);
			
		}
		
		return new DataResult($statement, $className);
		
	}
	
	/**
	 * Execute SQL with optional parameters and return the result
	 *
	 * @param string $sql The SQL to be executed
	 * @param array $parameters Optional array of PDO style parameters
	 * @return DataResult
	 */
	public function execute($sql, array $parameters = array()) {
		
		$statement = $this->_connection->prepare($sql);
		
		$paramNumber = 0;
		foreach($parameters as $k => $v) {
			
			if(is_int($k)) $paramNumber++;
			$param = is_int($k) ? $paramNumber : ':' . $k;
			
			$statement->bindParam($param, $parameters[$k]);
			
		}
		return $statement->execute();
		
	}
	
	public function __construct($dsn = null, $username = "root", $password = "", $options = null) {
		if(!is_null($dsn)) {
			$this->_connection = new \PDO($dsn, $username, $password, $options);
		} else {
			$this->_connection = self::$_defaultConnection;
		}
	}
	
	public static function select($sql = "*", $parameters = array()) {
		$da = new DataAdapter();
		$selector = new DataSelector($da);
		return $selector->select($sql, $parameters);
	}
	
	public static function getLastInsertId() {
		return self::$_defaultConnection->lastInsertId();
	}
	
	public static function insert($tableName, $fields, $updateFields = array()) {
		$da = new DataAdapter();
		
		$fieldsSql = '';
		$paramsSql = '';
		foreach($fields as $k => $val) {
			$fieldsSql .= ' `'.$k.'`,';
			$paramsSql .= ' :'.$k.',';
		}
		$fieldsSql = preg_replace('/,$/', '', $fieldsSql);
		$paramsSql = preg_replace('/,$/', '', $paramsSql);
				
		
		$query = "INSERT INTO `".$tableName."` (".$fieldsSql.") VALUES (".$paramsSql.")";
		
		if(is_array($updateFields) && count($updateFields) > 0) {
			$updateParams = array();
			$query .= " ON DUPLICATE KEY UPDATE ";
			foreach($updateFields as $k => $val) {
				$query .= '`'.$k.'` = :update_'.$k.', ';
				$updateFields['update_'.$k] = $val;
			}
			$query = preg_replace('/,\s$/', '', $query);
			$fields = array_merge($fields, $updateFields);
		}

		$q = $da->execute($query, $fields);
	}
	
	public static function update($tableName, $fields, $additionalSql = false, $parameters = array()) {
		$da = new DataAdapter();
		
		$query = "UPDATE `".$tableName."` SET ";
		foreach($fields as $k => $val) {
			$query .= " `".$k."` = :".$k.",";
		}
		$query = preg_replace('/,$/', '', $query);
				
		if($additionalSql) {
			$query .= " ".$additionalSql;
		}
		
		$parameters = array_merge($fields, $parameters);
		
		$q = $da->execute($query, $parameters);
		
		
	}
	
}

?>