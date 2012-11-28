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
 *
 * @author Jerome Dane
 *
 */
use BlankCanvas\gallery\Image;

class ImageController extends Controller {
	
	public function details($params = null) {

		if(isset($params['content'])) {
			$image = $params['content'];
		}
		return $this->view(array(
			'image' => $image
		));
	}
	public function create($params = null) {
	
		return $this->edit($params);
	}
	public function edit_form($params = null) {
		if(AppUser::can('gallery', 'edit_images')) {
			if(isset($params['id'])) {
				
			} else {
				$image = new gallery\Image();
			}
			
			if(isset($_FILES['image'])) {
				
				$image->type = preg_replace('/image\//', '', $_FILES['image']['type']);
				switch($image->type) {
					case 'jpeg': case 'png':
						if(isset($_POST['name']) && trim($_POST['name'])) {
							$image->name = (string) trim($_POST['name']);
						} 
						$image->save();
						move_uploaded_file($_FILES['image']['tmp_name'], 'data/gallery/images/'.$image->id.'.'.$image->type);
						
						
						if(isset($_POST['parent']) && $_POST['parent']) {
							DataAdapter::insert("system_nodes", array(
								"parent_id" => (int) $_POST['parent'],
								"content_id" => $image->id
							));
						}
						
						App::redirect($image->url);
						break;
					
				}
				
			}
			
			return $this->view($params);
		} else {
			return $this->_denied();
		}
	}
	public function edit_list_form($params = null) {
		$images = isset($params['images']) ? $params['images'] : array();
		
		if(isset($_POST['gallery_image_edit_list'])) {
			foreach($_POST as $k => $v) {
				if(preg_match('/^gallery_image_name_(\d+)$/', $k, $matches)) {
					if($image = Image::getById($matches[1])) {
						if(isset($_POST['gallery_image_delete_'.$image->id]) && $_POST['gallery_image_delete_'.$image->id]) {
							$image->delete();
						} else {
							$image->name = $v;
							$image->save();
						}
					}
				}
			}
		}
		
		return $this->view(array(
			"title" => isset($params['title']) ? $params['title'] : false,
			"images" => $images,
		));
	}
	public function edit($params = null) {
		
		if(isset($params['id'])) {
			
		} else {
			$image = new gallery\Image();
		}
		
		if(isset($_FILES['image'])) {
			
			$image->type = preg_replace('/image\//', '', $_FILES['image']['type']);
			switch($image->type) {
				case 'jpeg': case 'png':
					$image->save();
					move_uploaded_file($_FILES['image']['tmp_name'], 'data/gallery/images/'.$image->id.'.'.$image->type);
					
					if(isset($_POST['parent']) && $_POST['parent']) {
						DataAdapter::insert("system_nodes", array(
							"parent_id" => (int) $_POST['parent'],
							"content_id" => $image->id
						));
					}
					
					App::redirect($image->url);
					break;
				
			}
			
		}
		
		
		return $this->view(array(
			'image' => $image,
			'parent_id' => (isset($params['parent']) && preg_match('/^\d+$/', $params['parent'])) ? (int) $params['parent'] : 0
		));
	}
	
	public function thumbnail($params) {
		
		$image = $params['image'];
		
		return $this->view(array(
			'image' => $image
		));
	}
	
	
	
	
}

?>