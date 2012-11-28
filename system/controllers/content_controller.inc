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
class ContentController extends Controller {
	
	public function index($params = null) {
		if(preg_match('/^\d+$/', App::getRequestToken(0))) {
			$params['id'] = App::getRequestToken(0);
			return $this->details($params);
		} else {
			
			return $this->view();
		}
	}
	public function edit($params = null) {
		if(AppUser::can('system_edit_content')) {
			if($content = isset($params['content']) ? $params['content'] : Content::getById($params['id'])) {
				return $this->view(array(
					'content' => $content
				));
			} else {
				return $this->_404();
			}
		} else {
			return $this->_denied();
		}
	}
	public function details($params = null) {
		
		$customRendered = false;
		$content = Content::getById($params['id']);
		if($content && $content->content_type != 'system_content') {
			
			$className = $content->getClassName();
			$content = $className::getById($content->id);
			
			if(!$content) {
				return $this->_404();;
			}
			
			View::addCrumbs($content->getCrumbs());
			
			$controller = $content->getController();
			if($controller && method_exists($controller, 'details')) {
				$params['content'] = $content;
				View::setTitle($content->name);
				return $controller->details($params);
			}
			View::setTitle($content->name);
			return $this->view($content);
		} else {
			return $this->_404();
		}
	}
	public function create($params = null) {

		
		
		return $this->view();
	}
	
}

?>