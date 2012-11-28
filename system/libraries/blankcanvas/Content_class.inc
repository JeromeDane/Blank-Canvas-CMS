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
 * Content
 *
 * @author 	Jerome Dane
 *
 */
class Content extends DataModel {
	
	public function __construct($data = array()) {
		$this->_data = $data;
		if(is_null($this->_tableName)) {
			$this->_tableName = $this->_getType();
		}
		
		// check for orphaned/missing/broken system_content entry in non-standard content and create if necessary
		if($this->_getType() != 'system_content' && isset($this->_data['content_id']) && $this->_data['content_id'] && !$this->id) {
			$da = new DataAdapter();
			$da->insert('system_content', array(
				'id' => (int) $this->_data['content_id'],
				'content_type' => $this->_getType()
			));
			$this->id = (int) $this->_data['content_id'];
		}
		
	}
	
	public function updateType() {
		if(preg_match('/BlankCanvas\\\\Content$/', get_class($this)) && $this->content_type != 'system_content') {
			$className = $this->getClassName();
			return $className::getById($this->id);
		} else {
			return $this;
		}
	}
	
	public function getController() {
		preg_match('/^([a-z]+)_(.+)/i', $this->content_type, $matches);
		$context = $matches[1];
		$controllerName = $matches[2];
		return App::getController($context, $controllerName);
	}
	
	public function getParent() {
		
		$selector = Content::find("content_id = ".(int) $this->id)
			->join("system_nodes", "id = parent_id");
			
		if($selector->count()) {
			$content = $selector->fetch();
			if($content->content_type != 'system_content') {
					
				$className = $content->getClassName();
				$content = $className::getById($content->id);
				
			}
		} else {
			$content = false;
		}
		
		
			
		return  $content;
	}
	public function getChildren($type = false) {
		if($type && preg_match('/^([^_]+)_(.+)$/', $type, $matches)) {
			$className = '\\BlankCanvas\\'.$matches[1].'\\'.$matches[2];
		}
		$selector = new DataSelector(new DataAdapter(), ($type ? $className : null));
		
		$selector->select()->from("system_content")
			->join("system_nodes", "id = content_id")
			->where("parent_id = ".(int) $this->id);
		
		if($type) {
			$selector->join($type, $type.".content_id = id");
		}
		
		return $selector;
	}
	
	public function delete() {
		$da = new DataAdapter();
		$da->execute("DELETE FROM ".$this->_tableName." WHERE content_id = ".(int) $this->id);
		$da->execute("DELETE FROM system_content WHERE id = ".(int) $this->id);
		$da->execute("DELETE FROM system_nodes WHERE content_id = ".(int) $this->id);
	}
	
	public function save() {
		
		// make sure there's something new to save
		if(count($this->_modifiedData) > 0) {

			// calculate system and custom fields
			$sysFieldNames = array('id', 'name', 'description', 'body', 'created');
			$sysFields = array();
			$customFields = array();
			foreach($this->_modifiedData as $k => $v) {
				if(in_array($k, $sysFieldNames)) {
					$sysFields[$k] = $v;
				} else {
					$customFields[$k] = $v;
				}
			}
			
			// update existing content
			if(isset($this->_data['id'])) {
				if(count($customFields) || count($sysFields) > 0) {
					$sysFields['id'] = $this->_data['id'];
					$dateTime = new \DateTime();
					$sysFields['edited'] = $dateTime->format('Y-m-d H:i:s');
					DataAdapter::update('system_content', $sysFields, "WHERE id = :id");
				}
				if(count($customFields)) {
					$customFields['content_id'] = $this->_data['content_id'];
					DataAdapter::update($this->_tableName, $customFields, "WHERE content_id = :content_id");
				}
			} else {
				// insert new records
				$sysFields['content_type'] = $this->_getType();
				$dateTime = new \DateTime();
				$sysFields['created'] = $dateTime->format('Y-m-d H:i:s');
				$sysFields['creator_id'] = AppUser::getId();
				$sysFields['edited'] = $sysFields['created'];
				DataAdapter::insert('system_content', $sysFields);
				$id = DataAdapter::getLastInsertId();
				$type = $this->_getType();
				if($type != 'system_content') {
					$customFields['content_id'] = $id;
					DataAdapter::insert($this->_tableName, $customFields);
				}
				$this->id = $id;
				$this->content_id = $id;
				
			}
		}
	}
	
	public function __get($key) {
		switch($key) {
			case 'content_type':
				$var = isset($this->_data['content_type']) ? $this->_data['content_type'] : self::_getType();
				return $var;
				break;
			case 'url':
				$url = App::getRootUri() . $this->id .'/_' . (preg_replace('/[^\w]/', '_', $this->name));
				return $url;
				break;
			default:
				$var = isset($this->_data[$key]) ? $this->_data[$key] : null;
				return $var;
				
		}
	}
	
	public function getCrumbs() {
		return array(
			$this->name => $this->url
		);
	}
	
	public function getClassName() {
		$type = $this->content_type;
		preg_match('/^([a-z]+)_(.+)/i', $type, $matches);
		$context = $matches[1];
		$modelName = $matches[2];
		return $className = '\\BlankCanvas\\'.$context.'\\'.$modelName;
	}
	
	public static function getById($id) {
		return self::findSingle('id = '.(int) $id);
	}
	
	public static function find($condition = '', $params = array()) {
		
		$type = self::_getType();
		
		$q = new DataSelector(new DataAdapter(), get_called_class());
		if($type != 'system_content') {
			// get non standard content
			$q = $q->select()->from($type);
			// Use left join to account for broken/missing system_content entry. The constroctor will fix any errors.
			$q = $q->join('system_content', 'system_content.id = '.$type.'.content_id', false);
		} else {
			// get standard system content
			$q = $q->select()->from('system_content');
		}
		if(preg_match('/^\d+$/', $condition)) {
			$q = $q->where('system_content.id = '.(int) $condition);
		} else if($condition) {
			$q = $q->where($condition, $params);
		}
		return $q;
	}
	public static function findSingle($condition, $params = array()) {
		$content = self::find($condition, $params)->fetch();
		if($content) {
			return $content->updateType();
		} else {
			return null;
		}
	}
}

?>